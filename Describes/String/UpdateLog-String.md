# 此为更新日志，记录每次更新内容，并以版本号为索引
当前版本号：[1.10.3]
- String（自删除缺陷起）
    ## 添加
        [1.0.0] 添加拷贝构造函数
        [1.0.0] 添加移动构造函数
        [1.0.0] 添加非静态成员at函数
        [1.0.0] 添加非静态成员element函数
        [1.0.0] 添加非静态成员empty函数
        [1.0.0] 添加非静态成员bool操作符重载函数
        [1.0.0] 添加非静态成员函数：restore_cache_mode
            用于恢复动态字符串的状态至缓存模式
        [1.0.0] 添加非静态操作符重载成员函数==(...) |==|
            |==|: 添加参数为(char_t)的operator==重载
            |==|: 添加参数为(const_pointer_t)的operator==重载
        [1.0.0] 添加参数为(char_t)的构造函数
        [1.0.0] 添加非静态成员函数swap
        [1.0.0] 添加非静态成员函数move
        [1.1.0] 添加非静态操作符重载成员函数=(...) |=|
            |=|: 添加参数为(const_pointer_t)的operator=重载
            |=|: 添加参数为(basic_string&)的operator=重载
            |=|: 添加参数为(basic_string&&)的operator=重载
        [1.1.0] 添加非静态操作符重载成员函数+=(...) |+=|
            |+=|: 添加参数为(char_t)的operator+=重载
            |+=|: 添加参数为(const_pointer_t)的operator+=重载
            |+=|: 添加参数为(basic_string&)的operator+=重载
        [1.1.0] 添加非静态操作符重载成员函数+(...) |+|
            |+|: 添加参数为(char_t)的operator+重载
            |+|: 添加参数为(const_pointer_t)的operator+重载
            |+|: 添加参数为(basic_string&)的operator+重载
        [1.1.0] 添加参数为可编译期确认字符串的构造函数
        [1.1.1] 添加wrap并添加char_wrap
        [1.1.1] 添加dast并添加string
        [1.2.0] 添加非静态成员函数is_blank
        [1.2.0] 添加非静态成员函数is_digit
        [1.2.0] 添加非静态成员函数is_lower
        [1.2.0] 添加非静态成员函数is_upper
        [1.2.0] 添加非静态成员函数is_enstr
        [1.3.0] 添加非静态成员函数cut
        [1.3.0] 添加非静态成员函数reverse
        [1.3.0] 添加非静态成员函数trimmed
        [1.3.2] 为char工具的strset添加编译期运算
        [1.3.6] 对char工具的strset的编译期块添加提前返回
        [1.4.0] 添加非静态成员函数lower
        [1.4.0] 添加非静态成员函数upper
        [1.5.0] 为所有entrusted所调用的lambda添加constexpr noexcept属性
        [1.5.0] 添加非静态成员函数to_lower
        [1.5.0] 添加非静态成员函数to_upper
        [1.6.0] 添加非静态成员函数center
        [1.6.0] 添加非静态成员函数left
        [1.6.0] 添加非静态成员函数right
        [1.6.1] 为basic接口的成员函数pointer添加const尾部属性的实现以支持编译期
        [1.6.1] 为basic接口的成员函数begin添加const尾部属性的实现，并于core中实现const与非cosnt接口，以支持编译期
        [1.6.1] 为basic接口的成员函数end添加const尾部属性的实现，并修改core实现的接口的显示this可为const的，以支持编译期
        [1.6.1] 为basic接口的成员函数compare添加const尾部属性的实现，并修改basic接口的运算符重载成员函数==的尾部属性为const，以支持编译期
        [1.6.1] 为basic接口的运算符重载成员函数bool添加const尾部属性的实现以支持编译期
        [1.6.1] 为basic接口的运算符重载成员函数[]添加const属性的实现以支持编译期
        [1.6.1] 为string添加模板参数：SizeType，用于自定义size_t
        [1.6.1] 为char工具添加模板参数：SizeType，以符合string的SizeType
        [1.7.0] 添加非静态成员函数reserve
        [1.8.0] 为所有公开接口添加模板形参引用折叠推导的显式this形参
        [1.9.0] 添加非静态成员函数insert
        [1.9.3] 添加非静态成员函数expand_prefix
        [1.9.4] 添加非静态成员函数swapcase
        [1.9.4] 添加非静态运算符重载成员函数const_pointer_t
        [1.9.11] 添加非静态成员函数toggle_large_mode
        [1.9.20] 添加非静态成员函数is_ascii
        [1.9.21] 添加非静态成员函数exclusive_or
        [1.10.0] 于enhance模式为其operator+=添加加法器
        [1.10.0] 添加非静态成员函数append

    ## 删除
        [1.0.0] 将count成员删除
        [1.0.0] 删除string_core的构造函数
        [1.1.4] 删除沉余的以char_warp为类型的参数的构造函数，但是保留wrap
        [1.1.4] 删除沉余的静态成员函数strcopy
        [1.3.1] 删除operator+=的错误逻辑代码
        [1.3.1] 删除char工具的cpy方法的小块拷贝负优化
        [1.6.2] 删除参数为(basic_string& object, const_pointer_t pointer, size_t size)的非静态成员函数assign_init内部的错误变量声明

    ## 改动
        [1.0.0] 将相对应的构造函数转移至basic_string
        [1.0.0] 增加占位大小为7bit的count于buffer_t结构体内
        [1.0.0] 增加占位大小为1bit的cache于buffer_t结构体内
        [1.0.0] 为resize的参数fill添加默认值
        [1.0.0] 将sub功能函数集成于index函数，并删除非静态成员函数sub
        [1.0.0] 已实现增强模式下的operator=特性
        [1.0.0] 将比较函数的逻辑：先比较第一位字符更改为先比较最后一个字符
        [1.0.0] 更改释放的错误参数
        [1.0.0] 将相对应的非静态操作符重载成员函数转移至basic_string
        [1.0.0] 优化非静态成员函数const_string的性能
        [1.0.0] 优化非静态内部成员函数within_range的性能
        [1.0.0] 更改类的实现定义结构
        [1.0.0] 优化非静态成员函数size的性能
        [1.1.1] 将string的实现分块
        [1.1.1] 将原本的参数类型为char[]的编译期构造函数改为类型为wrap::char_wrap
        [1.1.1] 将string归类至dast
        [1.1.2] 更改非静态成员函数respace的逻辑错误：使用realloc更改为使用allocator重新申请内存
        [1.1.3] 根据文档Future的要求，对resize进行优化
        [1.1.3] 优化非静态成员函数mode_state的性能
        [1.1.3] 优化非静态成员函数end的性能
        [1.1.4] 优化非静态成员函数resize的性能
        [1.1.4] 对char工具的strcopy添加编译期分支预测，支持编译期调用
        [1.1.5] 优化非静态成员函数resize的性能
        [1.1.6] 更新restore_cache_mode的逻辑并优化其性能
        [1.3.0] 对于begin和end的core实现，也自basic调用
        [1.3.2] 优化char工具的strset的性能
        [1.3.2] 更改指定构造函数的错误代码
        [1.3.3] 改动operator+=的逻辑
        [1.3.4] 优化operator=(&&)的可读性
        [1.3.5] 更改operator==(const_pointer_t)的错误逻辑
        [1.4.0] 更改参数为(basic_string&)的构造函数的逻辑错误
        [1.4.0] 更改operator+=(...)的逻辑错误
        [1.6.1] 将core中的size，capacity，max_size接口的显示this引用改为显示this常量引用
        [1.6.3] 将非静态成员函数名：disconnect更改为truncate
        [1.6.4] 将非静态成员函数名：truncate更改为extract
        [1.6.5] 更新规范
        [1.6.5] 更改非静态成员函数resize的扩展行为
        [1.6.6] 改溯非静态成员函数resize的扩展行为
        [1.8.0] 诸多小改动不记录在更新列表
        [1.9.1] 更改非静态成员函数insert的逻辑错误
        [1.9.2] 针对部分行为进行assume属性明确（2025.12.1）
        [1.9.3] 诸多小改动不记录在更新列表
        [1.9.4] 修改非静态运算符重载成员函数operator bool为operator !
        [1.9.5] 诸多小改动不记录在更新列表
        [1.9.6] 优化成员函数exchange的代码可读性
        [1.9.7] 优化成员函数expand_prefix的代码可读性
        [1.9.8] 更正并修改了align（center， left，right）的代码逻辑
        [1.9.9] 修改了trimmed方法的文档
        [1.9.10] 修改了mode_state方法的文档
        [1.9.11] 将mode_status枚举类型通过“导出其它模块单元类型”（export using ::type）方法将mode_status在string.ixx导出
        [1.9.12] 优化了respace、move成员函数的代码可读性
        [1.9.13] 优化了析构函数的代码可读性
        [1.9.14] 优化了委派非静态成员函数assign(object)的性能
        [1.9.15] 更正了release非静态成员函数的逻辑
        [1.9.16] 更正了工具模块的字符工具子模块的若干代码
        [1.9.17] （再次.1）更正并修改了align（center， left，right）的代码逻辑
        [1.9.18] （再次.2）更正并修改了align（center， left，right）的代码逻辑
        [1.9.19] 优化了append成员函数的代码可读性
        [1.9.19] 更正并修改了move的代码逻辑
        [1.9.19] 更正并修改了append的代码逻辑
        [1.9.22] 诸多小改动不记录在更新列表
        [1.9.23] 诸多小改动不记录在更新列表
        [1.9.24] 将非静态成员函数mode_state命名更改为mode
        [1.10.1] 将非静态成员函数mode命名更改为info
        [1.10.2] 移除at函数的noexcept属性
        [1.10.3] 添加noexcept条件
    
    ## 保留
        [1.1.6] 参数为(size_t, Option)的resize暂不纳入更新列表，因为日后可能会删除它
        [1.4.0] 以注释的形式保留lower与upper的可读性实现
        [1.8.0] 有了显示this的属性推导之后可删除沉余的const属性的实现，但为了日后扩展，暂不删除实现，仅统一接


## Describes更新不记录于此日志中