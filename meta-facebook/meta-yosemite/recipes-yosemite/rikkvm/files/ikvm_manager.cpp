#include "ikvm_manager.hpp"

#include <thread>
#include <stdio.h>
#include <stdlib.h>

namespace ikvm
{

Manager::Manager(const Args& args) :
    continueExecuting(true), serverDone(false), videoDone(true),
    input(args.getKeyboardPath(), args.getPointerPath()),
    video(args.getVideoPath(), input, args.getFrameRate())/*,
    server(args, input, video)*/
{
	printf("iKVM VNC server-manager created\n");
}

void Manager::run()
{
    std::thread run(serverThread, this);

    //DEBUG only
    printf("iKVM video try to start\n");
    video.start();

    while (continueExecuting)
    {
        /*if (server.wantsFrame())
        {
            video.start();
            video.getFrame();
            server.sendFrame();
        }
        else
        {
            video.stop();
        }

        if (video.needsResize())
        {
            videoDone = false;
            waitServer();
            video.resize();
            server.resize();
            setVideoDone();
        }
        else
        {
            setVideoDone();
            waitServer();
        }*/
    }

    run.join();
}

void Manager::serverThread(Manager* manager)
{
	printf("iKVM thread started\n");
    /*while (manager->continueExecuting)
    {
        manager->server.run();
        manager->setServerDone();
        manager->waitVideo();
    }*/
}

void Manager::setServerDone()
{
    /*std::unique_lock<std::mutex> ulock(lock);

    serverDone = true;
    sync.notify_all();*/
}

void Manager::setVideoDone()
{
    /*std::unique_lock<std::mutex> ulock(lock);

    videoDone = true;
    sync.notify_all();*/
}

void Manager::waitServer()
{
    /*std::unique_lock<std::mutex> ulock(lock);

    while (!serverDone)
    {
        sync.wait(ulock);
    }

    serverDone = false;*/
}

void Manager::waitVideo()
{
    /*std::unique_lock<std::mutex> ulock(lock);

    while (!videoDone)
    {
        sync.wait(ulock);
    }*/

    // don't reset videoDone
}

} // namespace ikvm
