
26.5.19：
    
    1. 重构EditorCamera，继承SceneCamera，支持Perspective and Orthographic 分别在3D和2D场景中使用。

26.5.18：
    
    1. 为3D模型渲染添加阴影支持。

26.5.17：
    
    1. 修复粒子不会随着视口改变自适应


26.5.16：
    
    1. 为粒子组件与光源组件添加序列化，反序列化支持。

    2. 为圆圈渲染添加纹理支持

    3. 修复粒子发射器会在实体删除后调用的bug，现在使用场景指针遍历场景粒子更新。

26.5.15：
    
    1. 为粒子渲染添加多纹理支持

    2. 重构粒子渲染，使用geometry shader 生成粒子，并自动实现billboard效果。


26.5.14：
    
    1. 为模型渲染 添加 Point,Directional,Spot light source 

    2. 重构渲染逻辑，使shader在EndScene()中调用，减少资源浪费

26.5.13：

    1. 在cpu端添加视锥体剔除

    2. 添加模型AABB渲染

26.5.12：

    1. 添加ModelCache，不会再重复导入相同模型

26.5.11:

    1. 给粒子添加纹理

    2. 重构粒子渲染

26.5.10 ：

    1. 添加工具 SpriteSheetCropper, 分割纹理，并导出为 json 格式。

    2. 为 SpriteRendererComponent 添加 UV坐标，并重构 SpriteRendererComponent 渲染方式。

    3. 将 CubeComponent 改名为 LightComponent。

    4. 添加2D动画系统，根据SpritRendererComponent纹理地址获取 描述动画的json文件。

    5. 为粒子添加 Billboard 功能。

25.5.9 ： 

##完善粒子系统

    1. 添加 发射器形状(EmitterShap) : Point, Box, Sphere, Ring, Cone。

    2. 添加 爆发(burst) 为粒子系统。

    3. 为 粒子组件(ParticleComponent) 添加序列，反序列化方法。
    

25.5.8 : 
##初始化GT仓库