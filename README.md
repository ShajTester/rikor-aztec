# Rikor-Aztec

1. Clone
 ```bash
 $ git clone https://github.com/ShajTester/rikor-aztec.git
 $ cd rikor-aztec
 $ ./sync_yocto.sh
 ```

1. Initialize a build directory for the platform to build. In the `rikor-aztec` directory:
 ```bash
 $ source yo
 ```
1. Start the build within the build directory:
 In general to build for the platform:
 ```bash
 $ bitbake yosemite-image
 ```

## Kernel & U-Boot Development
By default build process fetches and build Linux kernel and U-boot directly from GitHub repository.
- To make local kernel changes and build with the modified kernel:

In the build directory, run
```
$ devtool modify linux-aspeed
```
or
```
$ devtool modify u-boot
```
This will create local Linux package under <buildir>/workspace/sources/linux-aspeed  for development

- To go back to default recipes, run
```
$ devtool reset linux-aspeed
```
