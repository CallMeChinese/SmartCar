# 开发记录

这里记载了在开发过程中发现的问题、做出的修改等内容，请按照第一个记录来记录下面的内容，时间格式为yyyy-MM-dd hh:mm

## 2019-12-21 02:28

发现的问题

+ PWM输出控制电机的杜邦线连接不稳定，操作的时候会出现接触不良的问题

+ 按钮开关Key2可能出现损坏或者焊接问题，单片机无法获得其状态

做出的改动

+ 在菜单中添加了小车的运行模式，模式0为正常模式，模式1为纯前进模式，即虽然做图像处理但是不改变速度，模式2为停止模式

+ 在菜单中添加了对于速度的控制，当选择前进速度、左编码器速度或者右编码器速度，且小车运行模式不在停止模式下时，可以通过上下按键来增大减小速度

+ 对先前的代码进行了少量的格式化，并删除了一些不可用的注释与变量
