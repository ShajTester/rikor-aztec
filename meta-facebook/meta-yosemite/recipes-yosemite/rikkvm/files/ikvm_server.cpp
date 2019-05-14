#include "ikvm_server.hpp"

#include <rfb/rfbproto.h>

//#include <phosphor-logging/elog-errors.hpp>
//#include <phosphor-logging/elog.hpp>
//#include <phosphor-logging/log.hpp>
//#include <xyz/openbmc_project/Common/error.hpp>

namespace ikvm
{

//using namespace phosphor::logging;
//using namespace sdbusplus::xyz::openbmc_project::Common::Error;

Server::Server(const Args& args, Input& i, Video& v) :
    pendingResize(false), frameCounter(0), numClients(0), input(i), video(v)
{
    printf("Try to start iKVM VNC server\n");
    std::string ip("localhost");                                    //TODO: ip should be modifyed
    const Args::CommandLine& commandLine = args.getCommandLine();
    int argc = commandLine.argc;

    server = rfbGetScreen(&argc, commandLine.argv, video.getWidth(),                //it is in vnclib main.c
                          video.getHeight(), Video::bitsPerSample,
                          Video::samplesPerPixel, Video::bytesPerPixel);

    if (!server)
    {
        /*log<level::ERR>("Failed to get VNC screen due to invalid arguments");
        elog<InvalidArgument>(
            xyz::openbmc_project::Common::InvalidArgument::ARGUMENT_NAME(""),
            xyz::openbmc_project::Common::InvalidArgument::ARGUMENT_VALUE(""));*/
        printf("The vnc server doesn't start\n");
    }

    framebuffer.resize(
        video.getHeight() * video.getWidth() * Video::bytesPerPixel, 0);

    server->screenData = this;
    server->desktopName = "OpenBMC IKVM";
    server->frameBuffer = framebuffer.data();
    server->newClientHook = newClient;
    server->cursor = rfbMakeXCursor(cursorWidth, cursorHeight, (char*)cursor,    //it is in vnclib cursor.c
                                    (char*)cursorMask);
    server->cursor->xhot = 1;
    server->cursor->yhot = 1;

    rfbStringToAddr(&ip[0], &server->listenInterface);                          //it is in vnclib sockets.c

    rfbInitServer(server);                                                      //it is in vnclib main.c

    rfbMarkRectAsModified(server, 0, 0, video.getWidth(), video.getHeight());   //it is in vnclib main.c

    server->kbdAddEvent = Input::keyEvent;
    server->ptrAddEvent = Input::pointerEvent;

    processTime = (1000000 / video.getFrameRate()) - 100;
}

Server::~Server()
{
    rfbScreenCleanup(server);                                                   //it is in vnclib main.c
}

void Server::resize()
{
    if (frameCounter > video.getFrameRate())
    {
        doResize();
    }
    else
    {
        pendingResize = true;
    }
}

void Server::run()
{
    rfbProcessEvents(server, processTime);                                      //it is in vnclib main.c

    if (server->clientHead)
    {
        input.sendReport();

        frameCounter++;
        if (pendingResize && frameCounter > video.getFrameRate())
        {
            doResize();
            pendingResize = false;
        }
    }
}

void Server::sendFrame()
{
    char* data = video.getData();
    rfbClientIteratorPtr it;
    rfbClientPtr cl;

    if (!data || pendingResize)
    {
        return;
    }

    it = rfbGetClientIterator(server);                                          //it is in vnclib rfbserver.c

    while ((cl = rfbClientIteratorNext(it)))                                    //it is in vnclib rfbserver.c
    {
        ClientData* cd = (ClientData*)cl->clientData;
        rfbFramebufferUpdateMsg* fu = (rfbFramebufferUpdateMsg*)cl->updateBuf;

        if (!cd)
        {
            continue;
        }

        if (cd->skipFrame)
        {
            cd->skipFrame--;
            continue;
        }

        if (cl->enableLastRectEncoding)
        {
            fu->nRects = 0xFFFF;
        }
        else
        {
            fu->nRects = Swap16IfLE(1);
        }

        fu->type = rfbFramebufferUpdate;
        cl->ublen = sz_rfbFramebufferUpdateMsg;
        rfbSendUpdateBuf(cl);                                                   //it is in vnclib rfbserver.c

        cl->tightEncoding = rfbEncodingTight;
        rfbSendTightHeader(cl, 0, 0, video.getWidth(), video.getHeight());      //it is in vnclib tight.c

        cl->updateBuf[cl->ublen++] = (char)(rfbTightJpeg << 4);
        rfbSendCompressedDataTight(cl, data, video.getFrameSize());             //it is in vnclib tight.c

        if (cl->enableLastRectEncoding)
        {
            rfbSendLastRectMarker(cl);                                          //it is in vnclib rfbserver.c
        }

        rfbSendUpdateBuf(cl);                                                   //it is in vnclib rfbserver.c
    }

    rfbReleaseClientIterator(it);                                               //it is in vnclib rfbserver.c
}

void Server::clientGone(rfbClientPtr cl)
{
    Server* server = (Server*)cl->screen->screenData;

    delete (ClientData*)cl->clientData;
    cl->clientData = nullptr;

    if (server->numClients-- == 1)
    {
        rfbMarkRectAsModified(server->server, 0, 0, server->video.getWidth(),   //it is in vnclib main.c
                              server->video.getHeight());
    }
}

enum rfbNewClientAction Server::newClient(rfbClientPtr cl)
{
    Server* server = (Server*)cl->screen->screenData;

    cl->clientData =
        new ClientData(server->video.getFrameRate(), &server->input);
    cl->clientGoneHook = clientGone;
    if (!server->numClients++)
    {
        server->pendingResize = false;
        server->frameCounter = 0;
    }

    return RFB_CLIENT_ACCEPT;
}

void Server::doResize()
{
    rfbClientIteratorPtr it;
    rfbClientPtr cl;

    framebuffer.resize(
        video.getHeight() * video.getWidth() * Video::bytesPerPixel, 0);

    rfbNewFramebuffer(server, framebuffer.data(), video.getWidth(),             //it is in vnclib main.c
                      video.getHeight(), Video::bitsPerSample,
                      Video::samplesPerPixel, Video::bytesPerPixel);
    rfbMarkRectAsModified(server, 0, 0, video.getWidth(), video.getHeight());   //it is in vnclib main.c

    it = rfbGetClientIterator(server);                                          //it is in vnclib rfbserver.c                                       

    while ((cl = rfbClientIteratorNext(it)))                                    //it is in vnclib rfbserver.c
    {
        ClientData* cd = (ClientData*)cl->clientData;

        if (!cd)
        {
            continue;
        }

        // delay video updates to give the client time to resize
        cd->skipFrame = video.getFrameRate();
    }

    rfbReleaseClientIterator(it);                                               //it is in vnclib rfbserver.c
}

} // namespace ikvm
