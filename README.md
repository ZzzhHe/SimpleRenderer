![github ci](https://github.com/Simple-XX/SimpleKernel/workflows/CMake/badge.svg)
![last-commit](https://img.shields.io/github/last-commit/google/skia.svg)
![languages](https://img.shields.io/github/languages/count/badges/shields.svg)
![MIT License](https://img.shields.io/github/license/mashape/apistatus.svg)

# SimpleRenderer

A software renderer

软件光栅化渲染器

缓冲
保存像素与深度信息，同时实现了 场景-模型-三角形-直线 四级绘制方法，
配置
保存了全局的信息 分辨率，绘制方法等
显示
将缓冲区的数据显示到屏幕上
光照
保存光线的颜色，位置，方向信息
模型
从 obj 文件读取数据保存到内存中，同时会计算法向量，碰撞盒
相机
相机的位置，方向，移动方式等
需要搞定 lookat 矩阵
着色
顶点着色与片元着色，
将 mvp 矩阵应用到每一个顶点上，
再使用片元着色确定像素的深度和颜色绘制三角形

输入 - 场景
将输入应用到场景上
根据场景的不同，使用的配置和输入不同
传入场景作为参数，在输入处理中对场景的相机、配置等进行修改
done
场景 - 配置 相机 输入 模型 光照
场景中包含了所有相机，模型，光照的信息，
绘制 - 场景 缓冲 着色
根据配置，使用指定的着色器将指定场景绘制到缓冲区中
渲染 - 缓冲 输入 场景 显示
将绘制好的缓冲区显示出来

读取模型，放入场景，初始化配置，相机

参考：https://github.com/ssloy/tinyrenderer

正在开发中...