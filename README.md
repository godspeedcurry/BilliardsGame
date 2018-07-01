# BilliardsGame

 

#### 看PDF github插入图片太麻烦了

本游戏是一款简单的双人对战的台球游戏。它包含了一款台球游戏所具有的基本功能，如台球击打、积分变换、积分系统等，也包含了其他攻击功能，如视角变换、犯规系统、力度控制、双人对战等。它采用鼠标操作与键盘操作相结合，有变换视角、声效等，较好的贴近了现实中的台球游戏。****

## [1.]()基本功能

**（1）游戏的环境：**游戏由台球桌面，灯光，以及状态板构成。初始情况下，白球放置在中线近侧，十五个彩球放在中线较远侧，几个球带与实际台球桌分布相符。状态板上有包括当前击球玩家，两位玩家的比分，以及击球力度等。

**（2）游戏的得分与扣分：**玩家将一个彩球击入球袋中后，玩家加一分；如果使白球落入袋中，则玩家扣一分，游戏恢复到上一个状态。

**（3）游戏的启动与退出：**玩家通过按下空格键（SPACE）可以重新开始一盘游戏，按下ESC键可以退出程序。

## [2.]()高级功能

**（1）视角变换：**通过上下左右键可以改变游戏中的视角，可以让玩家获得不同的击球视角，更准确的判断击球位置和击球方向，比二维游戏更加贴近真实情况。

**（2）力度控制：**玩家在选择好击球角度后，通过按下鼠标的长短控制击球的力度，相应的力度大小实时显示在状态栏中。

**（3）双人对战：**两名玩家轮流击球，如果一名玩家得分，则可再打一杆，否则交换球权。

** **

# [二．使用说明]()

## [1.]()安装说明

**（1）编译出可执行文件**

[OpenGL环境的安装：]()

**方法一：**

找到目录..\VC\Tools\MSVC\14.10.25017\include\gl(没有的话就建立一个)。 将glut.h放到里面。找到目录..\VC\Tools\MSVC\14.10.25017\lib\x86  将glut32.lib放到里面。最后把glut32.dll放到C:\Windows\SysWOW64(64位系统）。

**方法二（使用VS的插件）：**打开vs创建一个C++项目（Win32控制台-空白项目），然后点击 项目—管理Nuget程序包，然后点击浏览—在搜索栏输入NupenGL，然后安装（如果有两个就安装两个）。

运行下面的代码，得到相应的结果则证明已配置OpenGL环境成功。接着打开项目工程文件，生成解决方案，运行即可。

```c++
#include <GL/glut.h>

void Show()

{

    glClear(GL_COLOR_BUFFER_BIT);

    glRectf(-0.1f, -0.1f, 0.5f, 0.5f);

    glFlush();

}

 

int main(int argc, char *argv[])

{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB |GLUT_SINGLE);

    glutInitWindowPosition(100, 100);

    glutInitWindowSize(400, 400);

    glutCreateWindow("OpenGL");

    glutDisplayFunc(Show);

    glutMainLoop();

    return 0;

}

```





![img](https://github.com/godspeedcurry/BilliardsGame/blob/master/img/1.png) 

**（2）直接运行：**直接打开\src\Project4\x64\Debug\Project4.exe

## [2.]()使用手册

（1）双人模式：玩家1先击打球，每个球1分；

（2）犯规：白球进洞或者没有碰到彩色球，-1分；

（3）长按鼠标可以实现力度控制，最大100%；

（4）移动上下左右键可以变换视角，实现精确击打。

 

# [三．程序结构]()

1.opengl有大量的函数，生涩难懂，因此本任务多是照着教程一步步写下来的

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image004.jpg)和我们以前写的第三次图形库很像，先初始化然后注册一系列的回调函数

 

2.初始化完毕后开始绘制整个场景 我们对opengl的使用并不熟练，因此使用了网上的一些教程上的代码（截图是部分代码）

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image006.jpg)

**3.绘制台球桌和各种物件**

流程如下：

保存当前坐标系glPushMatrix();

平移（glTranslated），变换坐标系（glRotated），调用绘制函数(drawXXX)

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image008.jpg)恢复坐标系glPopMatrix();

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image010.jpg)

 

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image012.jpg) 

 

 

 

 

（drawCuboid设置参数1,1,1的效果）

**4.****交换球权**

如果击球之后符合换球权条件，那么我们将调用函数交换球权。

由于函数是一直处在循环里的，刚开始我没能理解所以一直实现不了，后来想起来CAD开发中用到的状态变量，发现确实很好用。

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image014.jpg)

**5.****记分牌绘制**

绘制物体是一个调整参数的过程

只要按照相对的位置，逐步调整参数的大小（x,y,z）最终得到我们满意的答案

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image016.jpg)

**6.****字体绘制**

Opengl没有内置函数，要自己写，我从网上获得资料后加到了程序中，并改成自己喜欢的样式

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image018.jpg)

**7.****速度显示，分数显示，当前玩家显示**

这些内容其实是一个东西，因此放到一块儿讲，为了封装良好，写成一个个独立的函数。然后调整参数显示到计分板上，任务就完成了。

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image020.jpg)

 

 

**8.模拟球的碰撞**

由于最多只有16个球，遍历所有球的开销是可以接受的。找到两个球，判断距离，相撞的更新速度。找到球和洞，判断距离，进洞的把他变成不可见。遇到白球进洞，放回中间，游戏恢复到上一个状态。

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image022.jpg)

** **

# [四．系统设计难点及其解决方法]()

## [1.]()碰撞中的物理问题

想要解决台球的碰撞问题，不得不涉及到一个问题，即非对心碰撞。

我们考虑理想环境下台球的碰撞问题

1)  **正碰，一球静止 一球运动**：很简单，交换速度即可;

2)  **正碰，两球都在运动:**可计算得两球碰撞后速度;

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image024.jpg)

如图所示，在光滑水平面上，质量为m1、m2的两球发生对心弹性碰撞，碰撞前速度分别为v1和v2 设碰撞后速度变为v1＇和v2 ＇在弹性碰撞过程中，分别根据动量守恒定律、机械能守恒定律得：

m1v1+ m2v2= m1v1 ＇+ m2v2 ＇     1

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image026.gif)m1v12 + ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image026.gif)m2v22 = ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image026.gif)m1v1 ＇2+ ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image026.gif)m2v2 ＇2      2

由1、2解得：

v1 ＇=![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image028.gif)   ; 

v2 ＇=![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image030.gif)   ;

 

3)  **斜碰,一动一静**;

根据动量守恒定律有：m1![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)1+ m2![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)2= m1![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)10

得： ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)1 + ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)2 =![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image034.gif)10 ；

两边平方得：v12+v22+ 2![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)1·![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)2 = v102    1 

又根据能量守恒定律有：![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image026.gif)m1v12 + ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image026.gif)m2v22 = ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image026.gif)m1v102 

得：                  v12+ v22 = v102         2

1-2得：              2![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)1·![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)2 =0

即                       ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)1⊥ ![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image032.gif)2

可见，碰撞后两球成直角离开。

 

4）**斜碰,两个都在动**;

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image036.jpg)

动量是向量，其在正交的两个方向上，互相守恒，我们选取两球圆心的直线为x轴，垂直于圆心直线的为y轴，如上图所述，

X轴上满足动量守恒：m1vx+ m2ux = m1vx ＇+ m2ux ＇

并假设两球碰撞是完全弹性碰撞，得：

vx ＇=![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image038.gif)   ; 

ux ＇=![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image040.gif)   ;

假设两球质量相等，则: vx ＇= ux , ux ＇= vx ,即交换速度

Y轴上两球分速度不变，vy＇= vy, uy ＇= uy ;

碰撞的调试出过很多问题，纸笔反复验算好多次，确实比较难写，也参考了网上一些源代码。

## [2.]()击球力度的控制：

可通过计时器回调函数实现，使用鼠标回调函数确定某个全局变量的值，再根据此此值和上次计时器函数使用时的此值确定鼠标的状态并加以操作，最终可实现：1鼠标左键一直处于按下状态时，白球初始速度不断增加2鼠标左键由按下转为松开状态时，运行击球函数3储存鼠标当前状态，为下次计时器回调提供依据。

相关代码：

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image042.jpg)

## [3.]()WINDOWS系统API的使用

在程序opengl的整体框架中引入了windows的api，实现从系统获得时间并且接入了系统媒体接口。导入的头文件有mmsystem.h和windows.h。

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image044.jpg)

由于水平有限这里只能简单的调用来播放声音

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image046.jpg)

更深入的还有播放不同格式的音频，实现暂停、循环，以及模拟双声道等

同属于windows.h下还利用了一个函数GetLocalTime(&time),当交换球权的时候，利用此函数把系统时间赋给结SYSTEMTIME结构体，调用其中的wSecond参数；同样的操作保持另一个结构体与系统时间同步，其中差值即为经过的时间，计算后显示在计分板上即可实现倒计时。

 

## [4.Github]()多人开发：VS环境

在网页版github里创建好仓库然后邀请组员加入，一个master分支一个develop分支（开发环境下最新版本），开发成功后合并到master分支

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image048.jpg)

使用方法：

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image050.jpg)

安装完后启用即可，然后设置一下密码，可视化界面操作很方便

![img](file:///C:/Users/MR51D6~1.BEA/AppData/Local/Temp/msohtmlclip1/01/clip_image052.jpg)

更改某个文件后提交至develop分支，测试完成后合并到master分支

# [五．本程序亮点和小组成就]()

1.3D模式下的台球，击球趣味性更强，控制更加方便灵活。

2.更加真实的碰撞效果，游戏体验更佳。

3.小组分工严密，每个人完成自己的部分，把函数都封装起来，方便相互调用。

 

# [六．不足之处]()

**1.台球碰撞不够真实：**本程序考虑的台球碰撞都是基于两个质点间的碰撞，没有考虑台球体积造成的影响，因此与真实的台球碰撞相比仍有一定差距。

**2.****击球点位置：**在球杆击打主球时，可以有不同的击球点。最常见的击球点 有3个，即高点、中心点和低点。此外还有6个比较常用的击球点，分别是左上点和右上点、左 侧点和右侧点、左下点和右下点。除了击中中心点，击中其他点时白球都会发生一定的旋转，因此会影响白球的运动及其与其他球碰撞时两球的状态，这一考虑过于复杂，因此我们的程序未能考虑击球点的变化

**3.****还有一些功能没有实现，**包括前面的开始界面、使用说明等界面以及已进球的显，台球轨迹预测等。

 

# [