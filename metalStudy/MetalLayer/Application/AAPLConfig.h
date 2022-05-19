/// 在主线程渲染，在绘制期间的 UI 事件更容易管理，因为 UIU 调用必须在主线程
/// 也可以后台渲染，由于事件可以在 GPU 异步处理、某些情况下 UI更快地响应
#define RENDER_ON_MAIN_THREAD 1


/// 渲染事件：启用后以 60 帧渲染；禁用后当 UI 请求重绘时才渲染
#define ANIMATION_RENDERING   1


/// 开启大小自适应：当视图 size 改变时更新视图
/// 关闭后，当视图 size 改变时、需要显式更新视图
#define AUTOMATICALLY_RESIZE  1


/// 开启深度测试：渲染器创建深度缓冲区
#define CREATE_DEPTH_BUFFER   1

