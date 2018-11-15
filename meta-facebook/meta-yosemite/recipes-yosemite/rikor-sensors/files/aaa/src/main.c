/*
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 * Contributor(s): Jiri Hnidek <jiri.hnidek@tul.cz>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <stdarg.h>
#include <limits.h>
#include "iniparser.h"
#include "main.h"

static int running = 0;
static int delay = 1;
static int counter = 0;
static char *conf_file_name = NULL;
static char *pid_file_name = NULL;
static int pid_fd = -1;
static char *app_name = NULL;

// http://digitalchip.ru/osobennosti-ispolzovaniya-extern-i-static-v-c-c
// Однако иногда бывает нужно, чтобы глобальная переменная или функция 
// были видны только в том файле, где определены. 
// Это позволяет сделать атрибут static
FILE *log_stream;
FILE *text_result;

daemon_cfg_t daemon_cfg;



/*-------------------------------------------------------------------------*/
/**
  @brief    Error callback for iniparser: wraps `fprintf(log_stream, ...)`.
 */
/*--------------------------------------------------------------------------*/
static int iniparser_error_callback(const char *format, ...)
{
  int ret;
  va_list argptr;
  va_start(argptr, format);
  ret = vfprintf(log_stream, format, argptr);
  va_end(argptr);
  return ret;
}



/**
 * \brief Read configuration from config file
 */
int read_conf_file(int reload)
{
    dictionary *ini;
    int ret;
    char param_str[20];
    const char *name_str;
    char cwd[PATH_MAX];

	if (conf_file_name == NULL) return 0;

    iniparser_set_error_callback(iniparser_error_callback);

    ini = iniparser_load(conf_file_name);

	if (ini == NULL) 
	{
		syslog(LOG_ERR, "Can not open config file: %s, error: %s",
				conf_file_name, strerror(errno));
		return -1;
	}

	delay = iniparser_getint(ini, "main:update_delay", 10);
	ret = iniparser_getint(ini, "main:output_format", -1);

	name_str = iniparser_getstring(ini, "main:output_file", "/tmp/out.txt");
	if(daemon_cfg.datafilepath != NULL) free(daemon_cfg.datafilepath);
	daemon_cfg.datafilepath = malloc(strlen(name_str) + 1);
	strcpy(daemon_cfg.datafilepath, name_str);

	name_str = iniparser_getstring(ini, "main:test_path", "");
	if(daemon_cfg.basepath != NULL) free(daemon_cfg.basepath);
	if(strlen(name_str) == 0)
	{
		daemon_cfg.basepath = malloc(48);
		strcpy(daemon_cfg.basepath, "/sys/devices/platform/ast_adc.0");
	}
	else
	{
		getcwd(cwd, sizeof(cwd));
		strcat(cwd, "/");
		strcat(cwd, name_str);
		daemon_cfg.basepath = malloc(strlen(cwd) + 1);
		strcpy(daemon_cfg.basepath, cwd);
	}

	// for(int i=0; i<16; i++)
	// {
	// 	sprintf(param_str, "adc%d:name", i);
	// 	name_str = iniparser_getstring(ini, param_str, "-");
	// 	if(adcchndata[i].name != NULL) free(adcchndata[i].name);
	// 	adcchndata[i].name = malloc(strlen(name_str) + 1);
	// 	strcpy(adcchndata[i].name, name_str);

	// 	sprintf(param_str, "adc%d:r1", i);
	// 	adcchndata[i].r1 = iniparser_getint(ini, param_str, 0);
	// 	sprintf(param_str, "adc%d:r2", i);
	// 	adcchndata[i].r2 = iniparser_getint(ini, param_str, 1);
	// 	sprintf(param_str, "adc%d:v2", i);
	// 	adcchndata[i].v2 = iniparser_getint(ini, param_str, 0);
	// }

	if (ret > 0)
	{
	    iniparser_dump(ini, log_stream);

		if (reload == 1) 
		{
			syslog(LOG_INFO, "Reloaded configuration file %s of %s",
				conf_file_name,
				app_name);
		} 
		else 
		{
			syslog(LOG_INFO, "Configuration of %s read from file %s",
				app_name,
				conf_file_name);
		}
	}

    iniparser_freedict(ini);

	return 0;
}



/**
 * \brief Print help for this application
 */
void print_help(void)
{
	printf("Usage: %s [OPTIONS]\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help                 Print this help\n");
	printf("   -c --conf_file filename   Read configuration from the file\n");
	printf("   -l --log_file  filename   Write logs to the file\n");
	printf("\n");
}

/* Main function */
int main(int argc, char *argv[])
{
	// Инициализация

	static struct option long_options[] = {
		{"conf_file", required_argument, 0, 'c'},
		{"log_file", required_argument, 0, 'l'},
		{"help", no_argument, 0, 'h'},
		{NULL, 0, 0, 0}
	};
	int value, option_index = 0, ret;
	char *log_file_name = NULL;
	int start_daemonized = 0;

	app_name = argv[0];

	/* Try to process all command line arguments */
	while ((value = getopt_long(argc, argv, "c:l:h", long_options, &option_index)) != -1) {
		switch (value) {
			case 'c':
				conf_file_name = strdup(optarg);
				break;
			case 'l':
				log_file_name = strdup(optarg);
				break;
			case 'h':
				print_help();
				return EXIT_SUCCESS;
			case '?':
				print_help();
				return EXIT_FAILURE;
			default:
				break;
		}
	}

	/* Open system log and write message to it */
	openlog(argv[0], LOG_PID|LOG_CONS, LOG_DAEMON);
	syslog(LOG_INFO, "Started %s", app_name);

	/* Try to open log file to this daemon */
	if (log_file_name != NULL) {
		log_stream = fopen(log_file_name, "a+");
		if (log_stream == NULL) {
			syslog(LOG_ERR, "Can not open log file: %s, error: %s",
				log_file_name, strerror(errno));
			log_stream = stdout;
		}
	} else {
		log_stream = stdout;
	}



    dictionary *ini;
    const char *sec;
    const char *keys[100];
    const char *val;
    char raw_val[100];
	FILE *pfh;

    iniparser_set_error_callback(iniparser_error_callback);
    ini = iniparser_load(conf_file_name);

    for(int i=0; i<iniparser_getnsec(ini); i++)
    {
    	sec = iniparser_getsecname(ini, i);
    	if(sec != NULL)
    	{
			iniparser_getseckeys(ini, sec, keys);
	    	for(int j=0; j<iniparser_getsecnkeys(ini, sec); j++)
	    	{
	    		if(keys[j] != NULL) 
    			{
    				val = iniparser_getstring(ini, keys[j], "");
    				if(val[0] != 0)
    				{
						if((pfh = fopen(val, "r")) != NULL)
						{
							fgets(raw_val, sizeof(raw_val), pfh);
							fclose(pfh);
							for(int i=strlen(raw_val); (raw_val[i] <= ' ') && (i>0); i--) raw_val[i] = 0;
						}
						else
						{
							strcpy(raw_val, "no file");
						}
						printf("%s\n", raw_val);
    				}
    			}
	    	}
	    }
    }



	/* Close log file, when it is used. */
	if (log_stream != stdout) {
		fclose(log_stream);
	}

	/* Write system log and close it. */
	syslog(LOG_INFO, "Stopped %s", app_name);
	closelog();



	/* Free allocated memory */

	/* Free allocated memory */
	if (conf_file_name != NULL) free(conf_file_name);
	if (log_file_name != NULL) free(log_file_name);
	if (pid_file_name != NULL) free(pid_file_name);

	return EXIT_SUCCESS;
}
