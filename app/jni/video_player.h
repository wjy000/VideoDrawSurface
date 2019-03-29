//
// Created by user on 2019/4/2.
//

#ifndef FFMPEGNATIVEWINDOW_FFMPEG_H
#define FFMPEGNATIVEWINDOW_FFMPEG_H

#include <jni.h>
#include <android/native_window.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};

#define AUTO_ROTATION_FRAME 0

namespace kk {
    class VideoPlayer {
    public:
        VideoPlayer();

        void SetSurface(ANativeWindow *surface) {
            if (nativeWindow != NULL) {
                ANativeWindow_release(nativeWindow);
                nativeWindow = NULL;
            }
            nativeWindow = surface;
        }

        void SetDataSource(const char *path) {
            mPreLoad = false;
            mFileName = path;
        }

        void SetCallBack(jmethodID callback);

        int PreLoad();

        int Play(JNIEnv *env, jobject obj);

        void Stop();

        void Close();

        bool IsPlaying() {
            return mPlaying;
        }

        ~VideoPlayer() {
            Close();
        }

    private:
        bool mPreLoad;
        //循环播放
        bool mPlaying;
        bool mClose;
        int mRotation;
        int mVideoStream;
        int mPlayWidth;
        int mPlayHeight;
        ANativeWindow *nativeWindow = NULL;
        AVFormatContext *pFormatCtx = NULL;
        AVCodecContext *pCodecCtx = NULL;
        AVCodec *pCodec = NULL;
//        AVCodecParserContext *pParserCtx = NULL;
        AVFrame *pFrameRGBA = NULL;
        AVFrame *pFrameNv21 = NULL;
        AVFrame *pFrame = NULL;
        AVFrame *pRotationFrame = NULL;
        struct SwsContext *pRGBASwsCtx = nullptr;
        struct SwsContext *pNv21SwsCtx = nullptr;

        void OnCallBack(JNIEnv *env, jobject obj, AVFrame *frameNv21, int width, int height);

        jmethodID mCallBackId = nullptr;
        const char *mFileName = NULL;

        struct SwsContext *
        createRGBASwsContext(AVPixelFormat src_fmt, int src_width, int src_height,
                             AVFrame *frameRGBA, int width, int height);

        struct SwsContext *
        createNv21SwsContext(AVPixelFormat src_fmt, int src_width, int src_height,
                             AVFrame *frameNv21, int width, int height);
    };
}

#endif //FFMPEGNATIVEWINDOW_FFMPEG_H
