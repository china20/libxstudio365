#ifndef _NX_WXAPIOBJECTS_H_
#define _NX_WXAPIOBJECTS_H_
#include <vector>
#include <string>
#include "thelib/NXLibDefs.h"

/*! @brief 错误码
 *
 */
enum  NXWxErrCode {
    
    kWxErrCodeSuccess    = 0,    /**< 成功    */
    kWxErrCodeCommon     = -1,   /**< 普通错误类型    */
    kWxErrCodeUserCancel = -2,   /**< 用户点击取消并返回    */
    kWxErrCodeSentFail   = -3,   /**< 发送失败    */
    kWxErrCodeAuthDeny   = -4,   /**< 授权失败    */
    kWxErrCodeUnsupport  = -5,   /**< 微信不支持    */
};

/*! @brief 请求发送场景
 *
 */
enum NXWxSceneType {
    
    kWxSceneSession  = 0,        /**< 聊天界面    */
    kWxSceneTimeline = 1,        /**< 朋友圈      */
    kWxSceneFavorite = 2,        /**< 收藏       */
};

/*! @brief 该类为微信终端SDK所有请求类的基类
 *
 */
struct NXWxBaseReq
{
    /** 请求类型 */
    int type;
};

/*! @brief 该类为微信终端SDK所有响应类的基类
 *
 */
struct NXWxBaseResp {
    /** 错误码 */
    int errCode;
    /** 错误提示字符串 */
    std::string errStr;
    /** 响应类型 */
    int type;
};

class NXWxMediaMessage; // fwd

/*! @brief 第三方程序发送消息至微信终端程序的接口
 *
 * 第三方程序向微信发送信息需要调用此接口，并传入具体请求类型作为参数。请求的信息内容包括文本消息和多媒体消息，
 * 分别对应于text和message成员。调用该方法后，微信处理完信息会向第三方程序发送一个处理结果。
 * @see SendMessageToWXResp
 */
struct NXWxSendMessageToWXReq : public NXWxBaseReq
{
    /** 发送消息的文本内容
     * @attention 文本长度必须大于0且小于10K
     */
    std::string text;
    /** 发送消息的多媒体内容
     * @see WXMediaMessage
     */
    NXWxMediaMessage* message;
    /** 发送消息的类型，包括文本消息和多媒体消息两种
     * @attention 两者只能选择其一，不能同时发送文本和多媒体消息
     */
    bool bText;
    
    /** 发送的目标场景，可以选择发送到聊天界面(WXSceneSession)、朋友圈(WXSceneTimeline)或收藏(WXSceneFavorite)。
     * @note 默认发送到聊天界面。
     * @see WXScene
     */
    int scene;
};

/*! @brief 第三方程序发送SendMessageToWXReq至微信，微信处理完成后返回的处理结果类型。
 *
 * 第三方程序向微信终端发送SendMessageToWXReq后，微信发送回来的处理结果，该结果用SendMessageToWXResp表示。
 */
struct NXWxSendMessageToWXResp : public NXWxBaseResp {
};

/*! @brief 微信终端向第三方程序请求提供内容请求类型。
 *
 * 微信终端向第三方程序请求提供内容，微信终端会向第三方程序发送GetMessageFromWXReq请求类型，
 * 需要第三方程序调用sendResp返回一个GetMessageFromWXResp消息结构体。
 */
struct NXWxGetMessageFromWXReq :  public NXWxBaseReq {
};

/*! @brief 微信终端向第三方程序请求提供内容，第三方程序向微信终端返回处理结果类型。
 *
 * 微信终端向第三方程序请求提供内容，第三方程序调用sendResp向微信终端返回一个GetMessageFromWXResp消息结构体。
 */
struct NXWxGetMessageFromWXResp :  public NXWxBaseResp {
    /** 向微信终端提供的文本内容
     * @attention 文本长度必须大于0且小于10K
     */
    std::string text;
    /** 向微信终端提供的多媒体内容。
     * @see WXMediaMessage
     */
    NXWxMediaMessage* message;
    /** 向微信终端提供内容的消息类型，包括文本消息和多媒体消息两种
     * @attention 两者只能选择其一，不能同时发送文本和多媒体消息
     */
    bool bText;
};

/*! @brief 微信通知第三方程序，要求第三方程序显示的消息结构体。
 *
 * 微信需要通知第三方程序显示或处理某些内容时，会向第三方程序发送ShowMessageFromWXReq消息结构体。
 * 第三方程序处理完内容后调用sendResp向微信终端发送ShowMessageFromWXResp。
 */
struct NXWxShowMessageFromWXReq :  public NXWxBaseReq{
    /** 微信终端向第三方程序发送的要求第三方程序处理的多媒体内容
     * @see WXMediaMessage
     */
    NXWxMediaMessage* message;
};

/*! @brief 微信通知第三方程序，要求第三方程序显示或处理某些消息，第三方程序处理完后向微信终端发送的处理结果。
 *
 * 微信需要通知第三方程序显示或处理某些内容时，会向第三方程序发送ShowMessageFromWXReq消息结构体。
 * 第三方程序处理完内容后调用sendResp向微信终端发送ShowMessageFromWXResp。
 */
struct NXWxShowMessageFromWXResp : NXWxBaseResp {
};

/*! @brief 微信终端打开第三方程序请求类型
 *
 *  微信向第三方发送的结构体，第三方不需要返回
 */
struct NXWxLaunchFromWXReq : NXWxBaseReq{
};


/*! @brief 多媒体消息结构体
 *
 * 用于微信终端和第三方程序之间传递消息的多媒体消息内容
 */
class NXWxMediaMessage  : public Ref{
    
public:
    static NXWxMediaMessage* message();
    
    /** 标题
     * @attention 长度不能超过512字节
     */
    std::string title;
    /** 描述内容
     * @attention 长度不能超过1K
     */
    std::string *description;
    /** 缩略图数据
     * @attention 内存大小不能超过32K
     */
    std::vector<char>   thumbData;
    /** 多媒体消息标签，第三方程序可选填此字段，用于数据运营统计等
     * @attention 长度不能超过64字节
     */
    std::string mediaTagName;
    /** 多媒体数据对象，可以为WXImageObject，WXMusicObject，WXVideoObject，WXWebpageObject等。 */
    void*        mediaObject;
    
    /*! @brief 设置消息缩略图的方法
     *
     * @param image 缩略图
     * @attention 内存大小不能超过32K
     */
public:
    void setThumbImage(const char* image);
    
};

/*! @brief 多媒体消息中包含的图片数据对象
 *
 * 微信终端和第三方程序之间传递消息中包含的图片数据对象。
 * @attention imageData和imageUrl成员不能同时为空
 * @see WXMediaMessage
 */
class NXWxImageObject : public Ref{
public:
    /*! @brief 返回一个WXImageObject对象
     *
     * @note 返回的WXImageObject对象是自动释放的
     */
    static NXWxImageObject* object();
    
    /** 图片真实数据内容
     * @attention 大小不能超过10M
     */
    std::vector<char> *imageData;
    /** 图片url
     * @attention 长度不能超过10K
     */
    std::string imageUrl;
    
};

/*! @brief 多媒体消息中包含的音乐数据对象
 *
 * 微信终端和第三方程序之间传递消息中包含的音乐数据对象。
 * @attention musicUrl和musicLowBandUrl成员不能同时为空。
 * @see WXMediaMessage
 */
class NXWxMusicObject  : public CCObject{
public:
    /*! @brief 返回一个WXMusicObject对象
     *
     * @note 返回的WXMusicObject对象是自动释放的
     */
    static NXWxMusicObject* object();
    
    /** 音乐网页的url地址
     * @attention 长度不能超过10K
     */
    std::string musicUrl;
    /** 音乐lowband网页的url地址
     * @attention 长度不能超过10K
     */
    std::string musicLowBandUrl;
    /** 音乐数据url地址
     * @attention 长度不能超过10K
     */
    std::string musicDataUrl;
    
    /**音乐lowband数据url地址
     * @attention 长度不能超过10K
     */
    std::string musicLowBandDataUrl;
    
};

/*! @brief 多媒体消息中包含的视频数据对象
 *
 * 微信终端和第三方程序之间传递消息中包含的视频数据对象。
 * @attention videoUrl和videoLowBandUrl不能同时为空。
 * @see WXMediaMessage
 */
class NXWxVideoObject  : public CCObject{
public:
    static NXWxVideoObject* object();
    /*! @brief 返回一个WXVideoObject对象
     *
     * @note 返回的WXVideoObject对象是自动释放的
     */
    // +(WXVideoObject *) object;
    
    /** 视频网页的url地址
     * @attention 长度不能超过10K
     */
    std::string videoUrl;
    /** 视频lowband网页的url地址
     * @attention 长度不能超过10K
     */
    std::string videoLowBandUrl;
};

/*! @brief 多媒体消息中包含的网页数据对象
 *
 * 微信终端和第三方程序之间传递消息中包含的网页数据对象。
 * @see WXMediaMessage
 */
class NXWxWebpageObject : public CCObject {
public:
    /*! @brief 返回一个WXWebpageObject对象
     *
     * @note 返回的WXWebpageObject对象是自动释放的
     */
    static NXWxWebpageObject* object(void);
    
    /** 网页的url地址
     * @attention 不能为空且长度不能超过10K
     */
    std::string webpageUrl;
    
};

/*! @brief 多媒体消息中包含的App扩展数据对象
 *
 * 第三方程序向微信终端发送包含WXAppExtendObject的多媒体消息，
 * 微信需要处理该消息时，会调用该第三方程序来处理多媒体消息内容。
 * @note extInfo和fileData不能同时为空
 * @see WXMediaMessage
 */
class NXWxAppExtendObject : public CCObject {
public:
    /*! @brief 返回一个WXAppExtendObject对象
     *
     * @note 返回的WXAppExtendObject对象是自动释放的
     */
    static NXWxAppExtendObject* object(void);
    
    /** App文件数据，该数据发送给微信好友，微信好友需要点击后下载数据，微信终端会回传给第三方程序处理
     * @attention 大小不能超过10M
     */
    // NSData
    std::vector<char>   fileData;
    
    /** 第三方程序自定义简单数据，微信终端会回传给第三方程序处理
     * @attention 长度不能超过2K
     */
    std::string extInfo;
    
    /**
     * @attention Deprecated
     */
    std::string  url;
    
};


/*! @brief 多媒体消息中包含的表情数据对象
 *
 * 微信终端和第三方程序之间传递消息中包含的表情数据对象。
 * @see WXMediaMessage
 */
class NXWxEmoticonObject : public CCObject {
public:
    
    /*! @brief 返回一个WXEmoticonObject对象
     *
     * @note 返回的WXEmoticonObject对象是自动释放的
     */
    static NXWxEmoticonObject* object();
    
    /** 表情真实数据内容
     * @attention 大小不能超过10M
     */
    //@property (nonatomic, retain) NSData
    std::vector<char> emoticonData;
    
};

/*! @brief 多媒体消息中包含的文件数据对象
 *
 * @see NXWxMediaMessage
 */
class NXWxFileObject : public CCObject {
public:
    
    /*! @brief 返回一个WXFileObject对象
     *
     * @note 返回的WXFileObject对象是自动释放的
     */
    static NXWxFileObject * object(void);
    
    /** 文件后缀名
     * @attention 长度不超过64字节
     */
    std::string  fileExtension;
    
    /** 文件真实数据内容
     * @attention 大小不能超过10M, //@property (nonatomic, retain) NSData
     */
    std::vector<char> fileData;
    
};

class NXWxApiDelegate {
    
public:
    virtual void onResp(const NXWxBaseResp& ) = 0;
    virtual void onReq(const NXWxBaseReq& ) = 0;
};

#endif




