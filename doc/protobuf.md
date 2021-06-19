**目录**
---

- [protobuf资料整理](#protobuf资料整理)
  - [编码规则](#编码规则)
    - [简单的消息](#简单的消息)
    - [Base 128 Varints 编码](#base-128-varints-编码)
    - [结构体消息](#结构体消息)
    - [更多值类型](#更多值类型)
      - [有符号整数](#有符号整数)
      - [非varint数字](#非varint数字)
      - [字符串](#字符串)
    - [嵌套类型消息](#嵌套类型消息)
    - [可选与重复元素](#可选与重复元素)
      - [打包重复字段](#打包重复字段)
    - [字段顺序](#字段顺序)
      - [提示](#提示)
  - [proto3语法](#proto3语法)
    - [定义消息类型](#定义消息类型)
    - [标量值类型](#标量值类型)
    - [默认值](#默认值)
    - [枚举类型](#枚举类型)
    - [其他消息类型](#其他消息类型)
    - [嵌套类型](#嵌套类型)
    - [更新消息类型](#更新消息类型)
    - [未知字段](#未知字段)
    - [任意类型](#任意类型)
    - [Oneof](#oneof)
    - [map](#map)
    - [包](#包)
    - [服务定义](#服务定义)
    - [json mapping](#json-mapping)
    - [选项](#选项)
    - [自定义类](#自定义类)
  - [protobuf生成c++代码](#protobuf生成c代码)
    - [编译器调用](#编译器调用)
    - [package](#package)
    - [消息](#消息)
    - [字段](#字段)
      - [单一的字段:](#单一的字段)
      - [单一字符串字段](#单一字符串字段)
      - [单一的枚举字段](#单一的枚举字段)
      - [单独的嵌入式类型](#单独的嵌入式类型)
      - [重复的数字类型](#重复的数字类型)
      - [重复的字符串字段](#重复的字符串字段)
      - [重复枚举类型](#重复枚举类型)
      - [重复的嵌套类型](#重复的嵌套类型)
      - [oneof数字字段](#oneof数字字段)
      - [oneof字符串字段:](#oneof字符串字段)
      - [oneof枚举字段](#oneof枚举字段)
      - [oneof嵌套类型](#oneof嵌套类型)
      - [map字段](#map字段)
    - [any](#any)
    - [oneof](#oneof-1)
    - [枚举](#枚举)
    - [扩展](#扩展)
    - [Arena Allocation](#arena-allocation)
    - [服务](#服务)
    - [插件](#插件)

# protobuf资料整理

## 编码规则

文件见: https://developers.google.cn/protocol-buffers/docs/encoding

### 简单的消息

```protobuf
message Test {
    optional int32 a = 1;
}
```
在项目中，你可以创建一个Test1消息并且设置a为150，将Test1消息序列化到输出缓冲区中。
你将得到如下二进制:
```cpp
08 96 01
```
后续会介绍为什么会编码为这样的二进制。

### Base 128 Varints 编码

要理解简单协议的编码规则，首先要明白varints。Varints是一种使用一个或多个字节序列化整数
的方法。 较小的数字占用较少的字节数。除了最后一个字节外，varint中的每个字节都设置了最高
有效位（msb）–这表明还会有其他字节。 每个字节的低7位用于以7位为一组存储数字的二进制补码
表示，最低有效组在前。

例如 -1: `0000 0001`只有一个字节，不需要msb位。
例如 300 `1010 1100 0000 0010`超过了一个字节。

如何确定 300 的 varint 为 `1010 1100 0000 0010`呢?

1.  首先需要将msb移除:
    ```cpp
        1010 1100 0000 0010     // varint
    =>   010 1100  000 0010     // 移除msb的varint
    ```

2.  以7bit 1组为单位反转，因为varint存储数字时最低有效位在前，反转后连接:
    ```cpp
         010 1100  000 0010     // 移除msb的varint
    =>   000 0010  010 1100     // 反转后的varint
    =>  00000100101100          // 合并后的varint
    =>  0000 0001 0010 1100     // 二进制数据
    =>  256 + 32 + 8 + 4 = 300  // 十进制数据
    ```

### 结构体消息

如您所知，协议缓冲区消息是一系列键值对。 消息的二进制版本仅使用字段的编号作为关键字-每个
字段的名称和声明的类型只能在解码端通过引用消息类型的定义（即.proto文件）来确定。

对消息进行编码时，键和值被串联到一个字节流中。 在对消息进行解码时，解析器需要能够跳过无
法识别的字段。 这样，可以将新字段添加到消息中，而不会破坏不知道它们的旧程序。 为此，有线
格式消息中每对的“键”实际上是两个值– .proto文件中的字段编号，加上有线类型，该类型仅提供
足够的信息来查找以下值的长度。 在大多数语言实现中，此键称为标签。

下列是可用的标签类型:
| Type | Meaning          | Used For                                                 |
| ---- | ---------------- | -------------------------------------------------------- |
| 0    | Varint           | int32, int64, uint32, uint64, sint32, sint64, bool, enum |
| 1    | 64-bit           | fixed64, sfixed64, double                                |
| 2    | Length-delimited | string, bytes, embedded messages, packed repeated fields |
| 3    | Start group      | groups (deprecated)                                      |
| 4    | End group        | groups (deprecated)                                      |
| 5    | 32-bit           | fixed32, sfixed32, float                                 |

流消息中的每个键都具有`(field_number << 3) | wire_type `，换句话说，数字的最后三位存
储标签类型。

现在再来看一个简单的例子。第一个bit始终是varint键，`08`或者删除了msb`000 1000`。
使用后三位最为标签类型`000`为0，并且右移3位得到字段的值为1。因此得到为Varint类型的1。

再看两个字节存储 值150:
```cpp
    1001 0110           // 二进制的150
=>  0000 0001 0001 0110 // 7bit分离后补全
=>  1001 0110 0000 0001 // 反转后变更msb位
=>  0x9601
```

### 更多值类型

#### 有符号整数

与标签0类型关联的所有协议缓冲区类型都被编码为varint。但是，在编码负数时，带符号的int
类型（sint32和sint64）与“标准” int类型（int32和int64）之间存在重要区别。 如果将int32
或int64用作负数的类型，则结果varint总是十个字节长–实际上，它被视为非常大的无符号整数。 
如果使用带符号类型之一，则生成的varint使用ZigZag编码，效率更高。

ZigZag编码将有符号整数映射为无符号整数，以便具有较小绝对值（例如-1）的数字也具有较小的
varint编码值。 这样做的方式是通过正整数和负整数来回“曲折”，以便将-1编码为1，将1编码为2，
将-2编码为3，依此类推， 可以在下表中看到：

| Signed Original | Encoded As |
| --------------- | ---------- |
| 0               | 0          |
| -1              | 1          |
| 1               | 2          |
| -2              | 3          |
| 2147483647      | 4294967294 |
| -2147483648     | 4294967295 |

换句话说对于n进行编码时:
sint32 `(n << 1) ^ (n >> 31)`
sint64 `(n << 1) ^ (n >> 63)`

第二个移位`(n >> 31)`为算术移位。因此 如果n为正，是一个全0位，n为负，是一个全1位。
解析sint32或者sint64时，其值将解码回原始带符号版本。

#### 非varint数字

非varint数值类型很简单– double和fixed64具有线类型1，它告诉解析器期望固定的64位数据块；
 同样，float和fixed32的导线类型为5，这告诉它期望使用32位。 
 在这两种情况下，值均以小端字节顺序存储。

#### 字符串

标签2类型(长度分割) 表示该值是varint编码的长度，后跟指定数量的数据字节。

例如:
```protobuf
message Test2 {
    optional string b = 2;
}
```

b的值设置成`"testing"`二进制分布为`12 07 [74 65 73 74 59 6e 67]`
[]括号中的为utf8的编码。

此处的`键`为`0x12`。

```cpp
    0x12
=>  0001 0010   // 二进制
=>  00010 010   // 重新组合位
=>  field_number = 2 wire_type = 2
```
`值`的长度为7,后面的7个字节为我们的字符串。

### 嵌套类型消息

嵌套类型定义如下:
```prorobuf
message Test3 {
    optional Test1 c = 3;
}
```

将Test1的字段设置为150。编码为`1a 03 08 96 01`。
最后三个字节`08 96 01`，与Test1案例中编码完全一致。嵌套定义类型的处理方式与字符串的处理
方式是一样的。

`键`为`1a`。

```cpp
    0x1a
=>  0001 1010   // 二进制
=>  00011 010   // 重新组合位
=>  field_number = 3 wire_type = 2
```

`值`为长度为3，后面的3个字节为嵌套类型数据。

### 可选与重复元素

如果在proto2的消息定义具有`repeated`元素(不带[packed=true]选项),则编码消息具有零个或
多个具有相同字段编号的键值对。这些重复的值不必连续出现。它们可能与其他字段交错。解析时，
元素之间的顺序会保留下来，尽管其他字段的顺序会丢失。
在proto3中，重复字段使用压缩编码。

对于proto3中任何非重复字段，或者proto2中的可选字段，编码的消息可能具有也可能没有具有该
字段编号的键值对。

通常，编码消息永远不会有一个以上非重复字段实例。 但是，解析器应处理实际情况。 对于数字类
型和字符串，如果同一字段多次出现，则解析器将接受它看到的最后一个值。 对于嵌入式消息字段，
解析器合并同一字段的多个实例，就像使用`Message::MergeFrom`方法一样-也就是说，后一个实
例中的所有奇异标量字段将替换前一个实例中的奇异标量字段，合并并重复 字段是串联的。 这些规
则的作用是，解析两个已编码消息的串联将产生与您分别解析两个消息并合并结果对象完全相同的结
果。 也就是说，这是：

```cpp
MyMessage message;
message.ParseFromString(str1 + str2);
```

等价于:
```cpp
MyMessage message, message2;
message.ParseFromString(str1);
message2.ParseFromString(str2);
message.MergeFrom(message2);
```

该属性有时很有用，因为即使您不知道它们的类型，它也允许您合并两个消息。

#### 打包重复字段

2.1.0版引入了打包重复字段，在proto2中声明为重复字段，但具有特殊的`[packed = true]`选项。
在proto3中，默认情况下打包标量数字类型的重复字段。 这些功能类似于重复的字段，但是编码方
式不同。 包含零元素的压缩重复字段不会出现在编码的消息中。 否则，该字段的所有元素都将打包
为标签类型为2（定界）的单个键值对。 每个元素的编码方式与通常相同，不同之处在于之前没有键。

例如:
```protobuf
message Test4 {
    repeated int32 d = 4 [packed = true];
}
```
假设构造了一个`Test4`，并且赋值为`3`,`270`,`86942`给d。我们将得到如下编码:
```cpp
    0x22            // key (field number 4, wire type 2)
    0x06            // payload size (6 bytes)
    0x03            // first element (varint 3)
    0x8E 02         // second element (varint 270)
    0x9E A7 05      // third element (varint 86942)
```
只能将原始数字类型(使用varint, 32位或64位标签类型)重复声明为打包。

请注意，尽管通常没有理由为一个打包的重复字段编码多个键值对，但是编码器必须准备好接受多个
键值对。 在这种情况下，应将有效负载串联在一起。 每对必须包含整数个元素。

协议缓冲区解析器必须能够解析被打包为打包的重复字段，就好像它们没有打包一样，反之亦然。 
这允许以向前和向后兼容的方式将[packed = true]添加到现有字段。

### 字段顺序

字段编号可以在.proto文件中以任何顺序使用。 选择的顺序对消息的序列化方式没有影响。

序列化消息时，对于如何写入其已知字段或未知字段没有保证的顺序。 序列化顺序是一个实现细节，
将来任何特定实现的细节都可能更改。 因此，协议缓冲区解析器必须能够以任何顺序解析字段。

#### 提示

-   不要假定序列化消息的字节输出是稳定的。 对于具有传递性字节字段表示其他序列化协议缓冲
    区消息的消息尤其如此。

-   默认情况下，在同一协议缓冲区消息实例上重复调用序列化方法可能不会返回相同的字节输出；
    即默认序列化不是确定性的。    

    -   确定性序列化只能保证特定二进制文件的字节输出相同。 字节输出可能会在二进制的不同版本之间变化。

-   对于协议缓冲区消息实例foo，以下检查可能会失败。

    -   `foo.SerializeAsString() == foo.SerializeAsString()`
    -   `Hash(foo.SerializeAsString()) == Hash(foo.SerializeAsString())`
    -   `CRC(foo.SerializeAsString()) == CRC(foo.SerializeAsString())`
    -   `FingerPrint(foo.SerializeAsString()) == FingerPrint(foo.SerializeAsString())`

-   这是一些示例场景，其中逻辑等效的协议缓冲区消息`foo`和`bar`可能序列化为不同的字节输出。

    -   `bar` 由一台旧服务器序列化，该服务器将某些字段视为未知字段。
    -   `bar` 由以不同编程语言实现的服务器序列化，并以不同顺序序列化字段。
    -   `bar` 具有以非确定性方式序列化的字段。
    -   `bar` 具有一个字段，用于存储协议缓冲区消息的序列化字节输出，该消息以不同的序列化。
    -   `bar` 由新服务器序列化，该服务器由于实现更改而以不同顺序序列化字段。
    -   `foo`和`bar`都是单个消息的串联，但是顺序不同。


## proto3语法

文件见: https://developers.google.cn/protocol-buffers/docs/proto3

### 定义消息类型

先看一段简单定义proto3类型的代码:
```protobuf
syntax = "proto3";

message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
}
```

-   注意
    -   需要注意第一行不能为空并且不能是注释，如果你不写，默认以proto2解析
    -   `SearchRequest`指定3个字段，每个字段包含在此类型消息中，包括名称和类型

-   指定字段类型

    上述例子中，所有的字段均为标量类型，两个整数和一个字符串。但是你也可以为字段指定复合
    类型，包括枚举与其他消息类型。

-   分配字段编号

    每一个字段都只能有一个唯一编号。这些编号是为了标识二进制格式的字段。一旦使用了消息
    类型，就不应该更改这些字段号。[1,15]将占用一个字节。[16,2047]将占用两个字节。具体
    的编码规则见 [编码规则](#编码规则)。所以应该为经常出现的消息元素保留数字1到15。留得
    足够的预留空间将来扩展用。

    编号最小为1，最大为 $2^{29} - 1$ 或者 $536,870,911$。

    不可以使用`19000`到`19999`(`FieldDescriptor::kFirstReservedNumber`到`FieldDescriptor::kLastReservedNumber`)，因为他们是为协议缓冲区实现保留的。

    不可以使用之前保留的字段号。

- 指定字段规则
  
  消息字段可以是以下之一:

  - 单一元素
    
    格式正确的邮件可以包含零个或一个此字段（但不能超过一个）。 
    这是proto3语法的默认字段规则。

  - `repeated`

    格式正确的消息，此字段可以重复任意次(包括零次)。重复值得顺序将保留。
    在proto3中，标量数字类型的重复字段使用打包编码。

-   添加更多消息类型

    多个消息可以定义在一个.proto文件中。
    ```protobuf
    message SearchRequest {
        string query = 1;
        int32 page_number = 2;
        int32 result_per_page = 3;
    }

    message SearchResponse {
        ...
    }
    ```

-   添加注释

    可以使用c/c++风格的`//`和`/*...*/`注释
    ```protobuf
    /* SearchRequest represents a search query, with pagination options to
     * indicate which results to include in the response. */

    message SearchRequest {
      string query = 1;
      int32 page_number = 2;  // Which page number do we want?
      int32 result_per_page = 3;  // Number of results to return per page.
    }
    ```

-   保留字段

    如果您通过完全删除字段或将其注释掉来更新消息类型，则将来的用户在自己对该类型进行更新
    时可以重用该字段号。 如果它们以后加载同一.proto的旧版本，则可能导致严重的问题，包括
    数据损坏，隐私错误等。 确保不会发生这种情况的一种方法是指定保留已删除字段的字段编号
    （和/或名称，这也可能导致JSON序列化问题）。 如果将来有任何用户尝试使用这些字段标识
    符，则协议缓冲区编译器将抱怨。
    ```protobuf
    message Foo {
        reserved 2, 15, 9 to 11;
        reserved "foo", "bar";
    }
    ```
    不能在同一保留语句中混用字段名称和字段编号。

### 标量值类型

| proto Type | C++ Type | Notes                                                                      |
| ---------- | -------- | -------------------------------------------------------------------------- |
| double     | double   |                                                                            |
| float      | float    |                                                                            |
| int32      | int32    | 使用可变长度编码。负数编码效率低。如果字段可能具有负值，请改用sint32       |
| int64      | int64    | 使用可变长度编码。负数编码效率低。如果字段可能具有负值，请改用sint36       |
| uint32     | uint32   | 使用可变长度编码。                                                         |
| uint64     | uint64   | 使用可变长度编码。                                                         |
| sint32     | int32    | 使用可变长度编码。 有符号的int值。 与常规int32相比，它们更有效地编码负数。 |
| sint64     | int64    | 使用可变长度编码。 有符号的int值。 与常规int64相比，它们更有效地编码负数。 |
| fixed32    | uint32   | 始终为四个字节。 如果值通常大于$2^{28}$，则比uint32更有效。                |
| fixed64    | uint64   | 始终为八个字节。 如果值通常大于$2^{56}$，则比uint64更有效。                |
| sfixed32   | int32    | 总是四个字节                                                               |
| sfixed64   | int64    | 总是八个字节                                                               |
| bool       | bool     |                                                                            |
| string     | string   | 字符串必须始终包含utf-8编码或7位ascii文本，并且不能超过$2^{32}$。          |
| bytes      | string   | 可以包含任何长度不超过$2^{32}$的任意字节序列。                             |

### 默认值

解析消息时，如果编码的消息不包含特定的单数元素，则解析对象中的相应字段将设置为该字段的
默认值。这些默认值是特定于类型的：
-   对于字符串，默认值为空字符串。
-   对于字节，默认值为空字节。
-   对于bools，默认值为false。
-   对于数值类型，默认值为零。
-   对于枚举，默认值是第一个定义的枚举值，该值必须为0。
-   对于消息字段，未设置该字段。它的确切值取决于语言。

重复字段的默认值为空（通常是适当语言中的空列表）。

请注意，对于标量消息字段，一旦解析了消息，就无法判断字段是显式设置为默认值（例如，布尔值
是否设置为false）还是根本没有设置：在定义消息类型时，应该记住这一点。例如，如果您不希望
某个行为在默认情况下也发生，那么不要让布尔值在设置为false时打开该行为。另请注意，如果
标量消息字段设置为其默认值，则不会在连接上序列化该值。

### 枚举类型

定义消息类型时，可能希望其中一个字段只包含预定义值列表中的一个。
例如，假设您想为每个SearchRequest添加一个`corpus`字段，其中的语料库可以是UNIVERSAL、
WEB、IMAGES、LOCAL、NEWS、PRODUCTS或VIDEO。您只需在消息定义中添加一个枚举，每个可能的
值都有一个常量，就可以做到这一点。
```protobuf
message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  Corpus corpus = 4;
}
```
如您所见，语料库枚举的第一个常量映射到零：每个枚举定义必须包含一个映射到零的常量作为其
第一个元素。这是因为：
-   必须有一个零值，以便我们可以使用0作为数字默认值。
-   零值必须是第一个元素，以便与proto2语义兼容，其中第一个枚举值始终是默认值。

可以通过将相同的值赋给不同的枚举常量来定义别名。为此，您需要将`allow_alias`选项设置为
true，否则协议编译器将在找到别名时生成错误消息。
```protobuf
message MyMessage1 {
  enum EnumAllowingAlias {
    option allow_alias = true;
    UNKNOWN = 0;
    STARTED = 1;
    RUNNING = 1;
  }
}
message MyMessage2 {
  enum EnumNotAllowingAlias {
    UNKNOWN = 0;
    STARTED = 1;
    // RUNNING = 1; // Uncommenting this line will cause a compile error 
                    // inside Google and a warning message outside.
  }
}
```

枚举器常量必须在32位整数的范围内。由于枚举值在编码上使用可变编码，因此负值效率很低，不
建议使用。可以在消息定义内定义枚举，如上面的示例所示，也可以在消息定义外定义枚举 
这些枚举可以在.proto文件中的任何消息定义中重用。
还可以使用语法`_MessageType_._EnumType_`将一条消息中声明的枚举类型用作另一条消息中
字段的类型。

当运行使用EnUM的.java中的协议缓冲区编译器时，生成的代码将有一个对应的枚举，用于Java或
C++，Python的一个特殊枚举类，用于创建运行时生成类中具有整数值的符号常量集合。

在反序列化过程中，无法识别的枚举值将保留在消息中，尽管反序列化消息时如何表示这些值取决于
语言。在支持开放的枚举类型的语言中，这些值的值超出指定符号的范围，例如C++和GO，未知的
枚举值只是作为其基础的整数表示形式存储。在具有封闭枚举类型的语言（如Java）中，枚举中的
大小写用于表示无法识别的值，并且可以使用特殊的访问器访问基础整数。在这两种情况下，如果
消息被序列化，则无法识别的值仍将与消息一起序列化。

-   保留值

    如果通过完全删除枚举条目或注释它来更新枚举类型，则将来的用户可以在自己更新该类型时
    重用该数值。如果以后加载相同.proto的旧版本，这可能会导致严重问题，包括数据损坏、
    隐私漏洞等。确保不会发生这种情况的一种方法是指定保留已删除条目的数值（和/或名称，
    这也可能导致JSON序列化问题）。如果将来有任何用户试图使用这些标识符，协议缓冲区编译器
    就会抱怨。您可以使用`max`关键字指定您的保留数值范围上升到可能的最大值。
    ```protobuf
    enum Foo {
        reserved 2, 15, 9 to 11, 40 to max;
        reserved "FOO", "BAR";
    }
    ```

### 其他消息类型

可以将其他消息类型用作字段类型。
例如，假设您希望在每个SearchResponse消息中包含结果消息–为此，您可以在同一.proto中定义
结果消息类型，然后在SearchResponse中指定类型为Result的字段：
```protobuf
message SearchResponse {
  repeated Result results = 1;
}

message Result {
  string url = 1;
  string title = 2;
  repeated string snippets = 3;
}
```

-   导入引用

    请注意，此功能在Java中不可用。
    在上面的示例中，结果消息类型与SearchResponse在同一个文件中定义–如果要用作字段类型
    的消息类型已经在另一个.proto文件中定义了呢？
    通过导入其他.proto文件，可以使用这些文件中的定义。要导入另一个.proto的定义，请在
    文件顶部添加一个import语句：
    ```protobuf
    import "myproject/other_protos.proto";
    ```

默认情况下，只能使用直接导入的.proto文件中的定义。但是，有时可能需要将.proto文件移动
到新位置。不用直接移动.proto文件并在一次更改中更新所有调用站点，现在可以在旧位置放置
一个伪.proto文件，使用import public概念将所有导入转发到新位置。任何导入包含
`import public`语句的proto的人都可以传递地依赖`import public`依赖项。例如：
```protobuf
// new.proto
// All definitions are moved here
```
```protobuf
// old.proto
// This is the proto that all clients are importing.
import public "new.proto";
import "other.proto";
```
```protobuf
// client.proto
import "old.proto";
// You use definitions from old.proto and new.proto, but not other.proto
```
可以在`client.proto`中使用`old.proto`和`new.proto`中的定义，但是不能使用`other.proto`。

协议编译器使用`-I`或者`--proto_path`标志在协议编译器命令行上指定的一组目录中搜索导入的
文件。如果没有给定标志，它将在调用编译器的目录中查找。通常，您应该将`--proto_path`标志
设置为项目的根目录，并对所有导入使用完全限定名。

可以导入proto2消息类型并在proto3消息中使用它们，反之亦然。但是，proto2枚举不能在
proto3语法中直接使用（如果导入的proto2消息使用它们也可以）。

### 嵌套类型

可以在其他消息类型中定义和使用消息类型。
```protobuf
message SearchResponse {
  message Result {
    string url = 1;
    string title = 2;
    repeated string snippets = 3;
  }
  repeated Result results = 1;
}
```

如果要在其父消息类型之外重用此消息类型，则可以通过`_Parent_._Type_`调用。
```protobuf
message SomeOtherMessage {
  SearchResponse.Result result = 1;
}
```

可以多层嵌套:
```protobuf
message Outer {       // Level 0
  message MiddleAA {  // Level 1
    message Inner {   // Level 2
      int64 ival = 1;
      bool  booly = 2;
    }
  }
  message MiddleBB {  // Level 1
    message Inner {   // Level 2
      int32 ival = 1;
      bool  booly = 2;
    }
  }
}
```

### 更新消息类型

如果现有消息类型不再满足您的所有需求（例如，您希望消息格式具有一个额外的字段），但是您仍
然希望使用以旧格式创建的代码，请不要担心！ 在不破坏任何现有代码的情况下更新消息类型非常
简单。 只要记住以下规则：

-   不要更改任何现有字段的字段编号。

-   如果添加新字段，则仍可以使用新生成的代码来解析使用“旧”消息格式通过代码序列化的任何
    消息。 您应记住这些元素的默认值，以便新代码可以与旧代码生成的消息正确交互。 同样，
    新代码创建的消息也可以由旧代码解析：旧的二进制文件在解析时只会忽略新字段。 

-   只要在更新后的消息类型中不再使用字段号，就可以删除字段。 您可能想要重命名该字段，
    或者添加前缀`OBSOLETE_`，或者保留该字段编号，以使.proto的将来用户不会意外重用该编号。

-   int32，uint32，int64，uint64和bool都是兼容的–这意味着您可以将字段从这些类型中的一
    种更改为另一种，而不会破坏向前或向后的兼容性。 如果从对应的类型不适合的协议缓冲区中
    解析出一个数字，则将获得与在C ++中将数字强制转换为该类型一样的效果（例如，如果将64
    位数字读为int32， 它将被截断为32位）。

-   sint32和sint64彼此兼容，但与其他整数类型不兼容。

-   string和bytes是兼容的，只要字节是有效的UTF-8。

-   如果字节包含消息的编码版本，则嵌入式消息与字节兼容。

-   fixed32与sfixed32兼容，fixed64与sfixed64兼容。

-   对于字符串，字节和消息字段，可选与重复兼容。 给定重复字段的序列化数据作为输入，如果
    期望该字段是可选的，则如果它是原始类型字段，则将采用最后一个输入值；如果是消息类型字
    段，则将合并所有输入元素。 请注意，这对于数字类型（包括布尔值和枚举）通常不安全。 
    重复的数字类型字段可以以打包格式序列化，当期望使用可选字段时，该格式将无法正确解析。

-   enum在连线格式方面与int32，uint32，int64和uint64兼容（请注意，如果值不合适，该值
    将被截断）。 但是请注意，客户端代码在反序列化消息时可能会以不同的方式对待它们：
    例如，无法识别的proto3枚举类型将保留在消息中，但是反序列化消息时如何表示这取决于
    语言。整数字段始终只是保留其值。

-   将单个值更改为新的oneof的成员是安全且二进制兼容的。 如果您确定一次没有一个代码设置
    多个字段，那么将多个字段移动到一个新字段中可能是安全的。 将任何字段移至现有字段都不安全。

### 未知字段

未知字段是格式正确的协议缓冲区序列化数据，表示解析器无法识别的字段。 例如，当旧二进制文
件使用新字段解析新二进制文件发送的数据时，这些新字段将成为旧二进制文件中的未知字段。

最初，proto3消息在解析过程中始终丢弃未知字段，但是在版本3.5中，我们重新引入了保留未知
字段以匹配proto2行为的功能。 在版本3.5和更高版本中，未知字段将在解析期间保留并包含在
序列化输出中。

### 任意类型

Any消息类型使您可以将消息用作嵌入式类型，而无需定义它们的.proto。  Any包含任意序列化的
消息（以字节为单位）以及URL，URL作为该消息的类型并解析为该消息的类型的全局唯一标识符。
 要使用Any类型，您需要`import google/protobuf/any.proto`。
```protobuf
import "google/protobuf/any.proto";

message ErrorStatus {
  string message = 1;
  repeated google.protobuf.Any details = 2;
}
```
给定消息类型的默认类型URL为`type.googleapis.com/_packagename_._messagename_`。

不同的语言实现将支持运行时库帮助程序以类型安全的方式打包和解压缩Any值-例如，在Java中，
Any类型将具有特殊的pack()和unpack()访问器，而在C++中则具有PackFrom()和UnpackTo()方法：
```cpp
// Storing an arbitrary message type in Any.
NetworkErrorDetails details = ...;
ErrorStatus status;
status.add_details()->PackFrom(details);

// Reading an arbitrary message from Any.
ErrorStatus status = ...;
for (const Any& detail : status.details()) {
  if (detail.Is<NetworkErrorDetails>()) {
    NetworkErrorDetails network_error;
    detail.UnpackTo(&network_error);
    ... processing network_error ...
  }
}
```

如果您已经熟悉proto2语法，则Any可以保存任意proto3消息，类似于可以`extensions`的proto2消息。

### Oneof

如果您有一则消息包含许多字段，并且最多可以同时设置一个字段，则可以使用oneof功能强制执行
此行为并节省内存。

一个字段类似于常规字段，但一个共享内存中的所有字段除外，并且最多可以同时设置一个字段。 
设置oneof中的任何成员会自动清除所有其他成员。

可以使用`case()`或者`WhichOneof()`来检查oneof中的任意一个值。

- 使用Oneof

  要在.proto中定义一个oneof，请使用oneof关键字，后跟您的oneof名称，在本例中为test_oneof：
  ```protobuf
  message SampleMessage {
    oneof test_oneof {
      string name = 4;
      SubMessage sub_message = 9;
    }
  }
  ```

  然后，将oneof字段添加到oneof定义。 您可以添加任何类型的字段，但`map`字段和`repeated`字段除外。

Oneof特点

- 设置oneof字段将自动清除oneof的所有其他成员。 因此，如果您设置了一个字段，则只有您设置
  的最后一个字段仍然有一个值。
  ```cpp
  SampleMessage message;
  message.set_name("name");
  CHECK(message.has_name());
  message.mutable_sub_message();   // Will clear name field.
  CHECK(!message.has_name());
  ```

- 如果解析器在解析缓冲区上遇到同一个对象的多个成员，则在解析的消息中仅使用最后看到的成员。

- oneof不能用作repeated

- 反射的api可以用于oneof的所有字段

- 如果将oneof字段设置为默认值（例如将int32 oneof字段设置为0），则将设置该oneof字段的
  “大小写”，并且该值将在缓冲区上序列化。

- 如果您使用的是C ++，请确保您的代码不会导致内存崩溃。 以下示例代码将崩溃，因为通过调用
  set_name（）方法已经删除了sub_message。
  ```cpp
  SampleMessage message;
  SubMessage* sub_message = message.mutable_sub_message();
  message.set_name("name");      // Will delete sub_message
  sub_message->set_...            // Crashes here
  ```

- 同样，在C ++中，如果您用一个awap（）交换两条消息，则每条消息将以另一个的oneof情况结束：
  在下面的示例中，msg1将具有sub_message，而msg2将具有名称。
  ```cpp
  SampleMessage msg1;
  msg1.set_name("name");
  SampleMessage msg2;
  msg2.mutable_sub_message();
  msg1.swap(&msg2);
  CHECK(msg1.has_sub_message());
  CHECK(msg2.has_name());
  ```

添加或删除字段之一时请多加注意。 如果检查oneof的值返回None / NOT_SET，则可能意味着尚未
设置oneof或已将其设置为oneof的不同版本中的字段。 由于无法知道缓存区上的未知字段是否是
oneof的成员，因此无法分辨出差异。

标签重用问题

- 将字段移入或移出oneof

  消息被序列化和解析后，您可能会丢失一些信息（某些字段将被清除）。 但是，您可以安全地将
  单个字段移动到新字段中，并且如果知道只设置了一个字段，则可以移动多个字段。

- 删除一个oneof字段并将其添加回去

  在消息被序列化和解析之后，这可能会清除您当前设置的oneof字段。

- 拆分或者合并oneof

  这与移动常规字段有类似的问题。

### map

如果要在数据定义中创建关联映射，则协议缓冲区提供了方便的快捷方式语法：
```protobuf
map<key_type, value_type> map_field = N;
```
其中key_type可以是任何整数或字符串类型（因此，浮点类型和字节除外的任何标量类型）。 
请注意，枚举不是有效的key_type。  value_type可以是除另一个映射以外的任何类型。

因此，例如，如果您想创建一个项目地图，其中每个Project消息都与一个字符串键相关联，则可以
这样定义它：
```protobuf
map<string, Project> projects = 3;
```

map特点
- 不支持`Extensions `。

- map不可以是`repeated`, `optional`, `required`。

- map的值的协议缓冲区排序和map的迭代排序是不确定的，因此您不能依赖于地图项的特定顺序。

- 为.proto生成文本格式时，map按键排序。 数字键按数字排序。

- 从协议缓冲区解析或合并时，如果存在重复的映射键，则使用最后看到的键。 从文本格式解析map
  时，如果键重 复，则解析可能会失败。

向后兼容

map的映射语法与一下语法等效，因此协议缓冲区不支持map结构也可以处理数据:
```protobuf
message MapFieldEntry {
  optional key_type key = 1;
  optional value_type value = 2;
}

repeated MapFieldEntry map_field = N;
```
任何支持映射的协议缓冲区实现都必须产生并接受上述定义可以接受的数据。

### 包

您可以在.proto文件中添加可选的`package`，以防止协议消息类型之间的名称冲突。
```protobuf
package foo.bar;
message Open { ... }
```
然后，您可以在定义消息类型的字段时使用包说明符：
```protobuf
message Foo {
  ...
  required foo.bar.Open open = 1;
  ...
}
```
包说明符影响生成的代码的方式取决于您选择的语言：

- 在C++中，生成的类包装在C++名称空间中。 例如，Open将位于名称空间`foo::bar`中。

请注意，即使package指令不直接影响所生成的代码（例如在Python中），仍强烈建议为.proto文件
指定软件包，否则可能会导致描述符中的命名冲突，并使proto不支持其他语言可移植。

软件包和名称解析

协议缓冲语言中的类型名称解析类似于C ++：首先搜索最内层的作用域，然后搜索下一个最内层的
作用，依此类推，每个包都被视为其父包“内在”。 领先的'.' （例如.foo.bar.Baz）表示从最
外面的范围开始。

协议缓冲区编译器通过解析导入的.proto文件来解析所有类型名称。 每种语言的代码生成器都知道
如何引用该语言中的每种类型，即使它具有不同的范围规则。

### 服务定义

如果要将消息类型与RPC（远程过程调用）系统一起使用，则可以在.proto文件中定义RPC服务接口，
并且协议缓冲区编译器将以您选择的语言生成服务接口代码和存根。 因此，例如，如果要使用接收
SearchRequest并返回SearchResponse的方法来定义RPC服务，则可以在.proto文件中进行如下定义：
```protobuf
service SearchService {
  rpc Search(SearchRequest) returns (SearchResponse);
}
```

默认情况下，协议编译器将生成一个名为SearchService的抽象接口和相应的stub实现。 stub将
所有调用转发到RpcChannel，后者又是一个抽象接口，您必须根据自己的RPC系统定义自己。 例如，
您可以实现一个RpcChannel，该序列将消息序列化并通过HTTP发送到服务器。 换句话说，生成的
存根提供了一个类型安全的接口，用于进行基于协议缓冲区的RPC调用，而无需将您锁定在任何特定的
RPC实现中。 因此，在C ++中，您可能会得到如下代码：
```cpp
using google::protobuf;

protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
SearchService* service;
SearchRequest request;
SearchResponse response;

void DoSearch() {
  // You provide classes MyRpcChannel and MyRpcController, which implement
  // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
  channel = new MyRpcChannel("somehost.example.com:1234");
  controller = new MyRpcController;

  // The protocol compiler generates the SearchService class based on the
  // definition given above.
  service = new SearchService::Stub(channel);

  // Set up the request.
  request.set_query("protocol buffers");

  // Execute the RPC.
  service->Search(controller, request, response, protobuf::NewCallback(&Done));
}

void Done() {
  delete service;
  delete channel;
  delete controller;
}
```

所有服务类也都实现Service接口，该接口提供了一种在不知道方法名称或其在输入和输出类型的情况
下调用特定方法的方法。 在服务器端，这可用于实现可用来注册服务的RPC服务器。
```cpp
using google::protobuf;

class ExampleSearchService : public SearchService {
 public:
  void Search(protobuf::RpcController* controller,
              const SearchRequest* request,
              SearchResponse* response,
              protobuf::Closure* done) {
    if (request->query() == "google") {
      response->add_result()->set_url("http://www.google.com");
    } else if (request->query() == "protocol buffers") {
      response->add_result()->set_url("http://protobuf.googlecode.com");
    }
    done->Run();
  }
};

int main() {
  // You provide class MyRpcServer.  It does not have to implement any
  // particular interface; this is just an example.
  MyRpcServer server;

  protobuf::Service* service = new ExampleSearchService;
  server.ExportOnPort(1234, service);
  server.Run();

  delete service;
  return 0;
}
```

### json mapping

Proto3支持JSON中的规范编码，从而使在系统之间共享数据更加容易。 下表按类型对编码进行了描述。

如果JSON编码的数据中缺少某个值，或者该值为null，则在解析为协议缓冲区时，它将被解释为适当
的默认值。 如果字段在协议缓冲区中具有默认值，则默认情况下会在JSON编码数据中将其省略以节省
空间。 一个实现可以提供选项，以在JSON编码的输出中发出具有默认值的字段。

| proto3                 | JSON          | JSON example                            | Notes |
| ---------------------- | ------------- | --------------------------------------- | ----- |
| message                | object        | {"fooBar": v, "g": null, …}             | ----- |
| enum                   | string        | "FOO_BAR"                               | ----- |
| map<K,V>               | object        | {"k": v, …}                             | ----- |
| repeated V             | array         | [v, …]                                  | ----- |
| bool                   | true, false   | true, false                             | ----- |
| string                 | string        | "Hello World!"                          | ----- |
| bytes                  | base64 string | "YWJjMTIzIT8kKiYoKSctPUB+"              | ----- |
| int32, fixed32, uint32 | number        | 1, -10, 0                               | ----- |
| int64, fixed64, uint64 | string        | "1", "-10"                              | ----- |
| float, double          | number        | 1.1, -10.0, 0, "NaN", "Infinity"        | ----- |
| Any                    | object        | {"@type": "url", "f": v, … }            | ----- |
| Timestamp              | string        | "1972-01-01T10:00:20.021Z"              | ----- |
| Duration               | string        | "1.000340012s", "1s"                    | ----- |
| Struct                 | object        | { … }                                   | ----- |
| Wrapper types          | various types | 2, "2", "foo", true, "true", null, 0, … | ----- |
| FieldMask              | string        | "f.fooBar,h"                            | ----- |
| ListValue              | array         | [foo, bar, …]                           | ----- |
| Value                  | value         |                                         | ----- |
| NullValue              | null          |                                         | ----- |
| Empty                  | object        | {}                                      | ----- |

JSON 选项
- 省略具有默认值的字段:

  默认情况下，proto3 JSON输出中会省略具有默认值的字段。 一个实现可以提供一个选项，以使用
  默认值覆盖此行为和输出字段。

- 忽略未知字段:

  Proto3 JSON解析器默认情况下应拒绝未知字段，但可以提供在解析时忽略未知字段的选项。

- 使用原型字段名称而不是lowerCamelCase名称:

  默认情况下，proto3 JSON打印机应将字段名称转换为lowerCamelCase并将其用作JSON名称。 
  一个实现可以提供一个选项，改为使用原型字段名称作为JSON名称。  Proto3 JSON解析器必须
  接受转换后的lowerCamelCase名称和原型字段名称。

- 将枚举值作为整数而不是字符串发送:

  默认情况下，JSON输出中使用枚举值的名称。 可以提供一个选项来代替使用枚举值的数字值。

### 选项

proto文件中的单个声明可以使用许多选项进行注释。选项不会更改声明的总体含义，但可能会影响
在特定上下文中处理声明的方式。可用选项的完整列表在`google/protobuf/descriptor.proto`。

以下是一些最常用的选项：

- `optimize_for `

  （文件选项）：可以设置为`SPEED`，`CODE_SIZE`或`LITE_RUNTIME`。
  这会通过以下方式影响C ++和Java代码生成器（可能还有第三方生成器）：

  - `SPEED`

    （默认值）：协议缓冲区编译器将生成代码，用于对消息类型进行序列化，解析和执行其他常见
    操作。 此代码已高度优化。

  - `CODE_SIZE`

    协议缓冲区编译器将生成最少的类，并将依赖于基于反射的共享代码来实现序列化，解析和其他
    各种操作。 因此，生成的代码将比使用SPEED的代码小得多，但是操作会更慢。 类仍将实现与
    SPEED模式下完全相同的公共API。 此模式在包含大量.proto文件且不需要所有文件都快速达到
    要求的应用程序中最有用。

  - `LITE_RUNTIME`

    协议缓冲区编译器将生成仅依赖于“精简版”运行时库的类（libprotobuf-lite而非libprotobuf）。 
    精简版运行时比完整库要小得多（大约小一个数量级），但省略了某些功能，例如描述符和反射。 
    这对于在受限平台（例如手机）上运行的应用程序特别有用。 编译器仍将像在SPEED模式下一样
    快速生成所有方法的实现。 生成的类将仅以每种语言实现MessageLite接口，该接口仅提供完整
    Message接口方法的子集。

  ```protobuf
  option optimize_for = CODE_SIZE;
  ```

- `cc_enable_arenas`

  （文件选项）：启用C++生成代码的Arena分配器。

- `deprecated`

  （字段选项）：如果设置为true，则表示该字段已弃用，并且不应被新代码使用。 在大多数语言中，
  这没有实际效果。 在Java中，这成为@Deprecated注释。 将来，其他特定于语言的代码生成器
  可能会在字段的访问器上生成弃用注释，这反过来将导致在编译尝试使用该字段的代码时发出警告。 
  如果该字段未被任何人使用，并且您想阻止新用户使用该字段，请考虑使用保留语句替换该字段声明。

  ```protobuf
  int32 old_field = 6 [deprecated = true];
  ```

自定义的选项:

协议缓冲区还允许您定义和使用自己的选项。 这是大多数人不需要的高级功能。 如果您确实需要创建
自己的选项，请参阅[proto2操作指南](https://developers.google.cn/protocol-buffers/docs/proto#customoptions)以了解详细信息。 请注意，创建自定义选项使用扩展名，
扩展名仅适用于proto3中的自定义选项。

### 自定义类

通过`protoc`生成定义在`.proto`中的消息，可以在C++等语言中可用的代码。
```shell
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR --go_out=DST_DIR --ruby_out=DST_DIR --objc_out=DST_DIR --csharp_out=DST_DIR path/to/file.proto
```

- `IMPORT_PATH`:

  指定解析导入指令时要在其中查找.proto文件的目录。 如果省略，则使用当前目录。 
  可以通过多次传递--proto_path选项来指定多个导入目录。 将按顺序搜索它们。  
  -I = _IMPORT_PATH_可以用作--proto_path的缩写形式。

- 您可以提供一个或多个输出指令:

  - `--cpp_out`

     在DST_DIR中生成C ++代码。 有关更多信息，请参见[C++生成的代码参考](https://developers.google.cn/protocol-buffers/docs/reference/cpp-generated)。

- 您必须提供一个或多个.proto文件作为输入。 可以一次指定多个.proto文件。 尽管这些文件是
  相对于当前目录命名的，但是每个文件都必须位于IMPORT_PATH之一中，以便编译器可以确定其规范名称。

## protobuf生成c++代码

文件见: https://developers.google.cn/protocol-buffers/docs/reference/cpp-generated#nested-types

### 编译器调用

当使用`--cpp_out=`命令行标志调用时，协议缓冲区编译器将生成C++输出。`--cpp_out=`选项的
参数是您希望编译器编写C++输出的目录。 编译器为每个.proto文件输入创建一个头文件和一个实现
文件。通过采用.proto文件的名称并进行两个更改来计算输出文件的名称：
- 扩展名（.proto）分别替换为标头或实现文件的`.pb.h`或`.pb.cc`。

- 原型路径（由`--proto_path=`或`-I`命令行标志指定）被输出路径（由`--cpp_out=`标志指定）替换。

例子如下:
```shell
protoc --proto_path=src --cpp_out=build/gen src/foo.proto src/bar/baz.proto
```
编译器将读取文件`src/foo.proto`和`src/bar/baz.proto`并生成四个输出文件：`build/gen/foo.pb.h`，
`build/gen/foo.pb.cc`，`build/gen/bar/baz.pb.h`，构建`/gen/bar/baz.pb.cc`。 必要时，
编译器将自动创建目录`build/gen/bar`，但不会创建`build`或`build/gen`。 它们必须已经存在。

### package

在.proto协议文件中如果定义了`package`，那么在C++中将被替换成namespace。
```protobuf
package foo.bar;
```
该文件中的所有定义将被放在`foo::bar`命名空间中。

### 消息

```protobuf
message Foo {}
```

协议缓冲区编译器会生成一个名为Foo的类，该类是从google :: protobuf :: Message公开派生的。 
该类是具体的类； 没有任何纯虚拟方法可以实施。 根据优化模式，在Message中为虚拟但不是纯虚拟
的方法可能会被Foo覆盖，也可能不会被Foo覆盖。 默认情况下，Foo实现所有方法的专用版本以实现
最大速度。 但是，如果.proto文件包含以下行：
```protobuf
option optimize_for = CODE_SIZE;
```
那么Foo将仅重写功能所需的最小方法集，并依赖其余方法的基于反射的实现。 这显着减小了所生成
代码的大小，但同时也降低了性能。 或者，如果.proto文件包含：
```protobuf
option optimize_for = LITE_RUNTIME;
```
那么Foo将包括所有方法的快速实现，但将实现google :: protobuf :: MessageLite接口，该接口
仅包含Message方法的一部分。 特别是，它不支持描述符或反射。 但是，在这种模式下，生成的代码
仅需要链接到libprotobuf-lite.so（在Windows上为libprotobuf-lite.lib），而不是
libprotobuf.so（libprotobuf.lib）。  “精简版”库比完整库小得多，并且更适合于资源受限的
系统，例如手机。

您不应该创建自己的Foo子类。 如果您将此类作为子类并覆盖虚拟方法，则该覆盖可能会被忽略，
因为许多生成的方法调用都已被虚拟化以提高性能。

[Message](https://developers.google.cn/protocol-buffers/docs/reference/cpp/google.protobuf.message#Message)接口定义了允许您检查，操作，读取或写入整个消息的方法，包括从二进制字符串进行解析和序列化。

- `bool ParseFromString(const string& data)`:

  从给定的序列化二进制字符串解析消息。

- ` bool SerializeToString(string* output) const`:

  将给定的消息序列化到二进制字符串中。

- `string DebugString()`:

  返回给出圆形的`test_format`表示形式的字符串（应仅用与调试）。

除了这些方法之外，Foo类还定义了以下方法：

- `Foo()`:

  默认构造函数

- `~Foo()`:

  默认析构函数

- `Foo(const Foo& other)`:

  拷贝构造函数

- `Foo& operator=(const Foo& other)`:

  拷贝赋值函数

- `void Swap(Foo* other)`:

  与其他消息交换

- `const UnknownFieldSet& unknown_fields() const`:

  返回解析此消息时遇到的未知字段集

- `UnknownFieldSet* mutable_unknown_fields()`:

  返回一个指针，该指针指向解析此消失遇到的可变未知字段集

该类还定义了以下静态方法：

- `static const Descriptor* descriptor()`:

  返回类型的描述符。 其中包含有关类型的信息，包括其具有的字段及其类型。 
  可以与反射一起使用，以编程方式检查字段。

- `static const Foo& default_instance()`:

  返回Foo的const单例实例，该实例与新构造的Foo实例相同（因此所有单数字段均未设置，所有
  重复字段均为空）。 请注意，消息的默认实例可以通过调用其New（）方法用作工厂。

嵌套类型

一条消息可以在另一条消息中声明。 例如：
```protobuf
message Foo { message Bar { } }
```

在这种情况下，编译器将生成两个类：`Foo`和`Foo_Bar`。 另外，编译器在Foo内部生成typedef，如下所示：
```cpp
typedef Foo_Bar Bar;
```
这意味着您可以像使用嵌套类`Foo::Bar`一样使用嵌套类型的类。 但是，请注意，C++不允许前向
声明嵌套类型。 如果要在另一个文件中向前声明Bar并使用该声明，则必须将其标识为`Foo_Bar`。

### 字段

除了上一节中介绍的方法外，协议缓冲区编译器还会为.proto文件中的消息内定义的每个字段生成
一组访问器方法。

与访问器方法一样，编译器还会为包含其字段号的每个字段生成一个整数常量。 常量名称是字母k，
其后是转换为驼峰大小写的字段名，然后是`FieldNumber`。 
例如，给定字段`int32 foo_bar = 5;`，编译器将生成常数`static const int kFooBarFieldNumber = 5;`。

对于返回const引用的字段访问器，当对该消息进行下一次修改访问时，该引用可能无效。 这包括
调用任何字段的任何非常量访问器，调用从Message继承的任何非常量方法或通过其他方式（例如，
通过使用消息作为Swap（）的参数）修改消息。 相应地，只有在此期间未对消息进行修改访问的
情况下，才保证在访问者的不同调用之间返回的引用的地址是相同的。

对于返回指针的字段访问器，当对该消息进行下一次修改或非修改访问时，该指针可能无效。 这
包括（无论是否为常数）调用任何字段的任何访问器，调用从Message继承的任何方法或通过其他方式
（例如，通过使用复制构造函数复制消息）访问消息。 相应地，决不能保证在访问器的两次不同调用
之间返回的指针的值是相同的。

#### 单一的字段:

proto2请自行查看。

在proto3中:
```protobuf
int32 foo = 1;
```
编译器将生成下列方法:
- `int32 foo() const`:

  返回字段的当前值。 如果未设置该字段，则返回0。

- `void set_foo(int32 value)`:

  设置字段的值。 调用此方法后，foo（）将返回值。

- `void clear_foo()`:

  清除字段的值。 调用此方法后，foo（）将返回0。

对于其他数字字段类型（包括bool），根据[标量值类型表](https://developers.google.cn/protocol-buffers/docs/proto3#scalar)将int32替换为相应的C++类型。

#### 单一字符串字段

proto2请自行查看。

在proto3中:
```protobuf
string foo = 1;
bytes foo = 1;
```
编译器将生成下列方法:
- `const string& foo() const`:

  返回字段的当前值。 如果未设置该字段，则返回空字符串/空字节。

- `void set_foo(const string& value)`:

  设置字段的值。 调用此方法后，foo（）将返回value的副本

- `void set_foo(string&& value)` C++11及更高版本:

  设置字段的值，从传递的字符串开始移动。调用此函数后，foo（）将返回值的副本。

- `void set_foo(const char* value)`:

  使用C样式的以null结尾的字符串设置字段的值。调用此函数后，foo（）将返回值的副本。

- `void set_foo(const char* value, int size)`:

  如上所述，但字符串大小是显式给定的，而不是通过查找空终止符字节来确定的。

- `string* mutable_foo()`:

  返回指向存储字段值的可变字符串对象的指针。如果在调用之前未设置该字段，则返回的字符串将
  为空。调用此函数后，foo（）将返回写入给定字符串的任何值。

- `void clear_foo()`:

  清除字段的值。调用此函数后，foo（）将返回空字符串/空字节

- `void set_allocated_foo(string* value)`:

  将字符串对象设置为字段，并释放上一个字段值（如果存在）。如果字符串指针不为空，则消息将
  获得分配的字符串对象的所有权。消息可以随时删除已分配的字符串对象，因此对该对象的引用
  可能会失效。否则，如果值为NULL，则行为与调用`clear_foo()`相同。

- `string* release_foo()`:

  释放字段的所有权并返回字符串对象的指针。调用此函数后，调用者将获得已分配字符串对象的所
  有权，foo（）将返回空字符串/空字节

#### 单一的枚举字段

proto2请自行查看。

在proto3中:
```protobuf
enum Bar {
  BAR_VALUE = 0;
  OTHER_VALUE = 1;
}
```
对于此字段定义:
```cpp
Bar foo = 1;
```
编译器将生成以下访问器方法：
- `Bar foo() const`:

  返回字段的当前值。如果未设置该字段，则返回默认值（0）。

- `void set_foo(Bar value)`:

  设置字段的值。调用此函数后，foo（）将返回值。

- `void clear_foo()`:

  清除字段的值。调用此函数后，foo（）将返回默认值。

#### 单独的嵌入式类型

proto2请自行查看。

在proto3中:
```protobuf
message Bar {}
```
定义:
```cpp
Bar foo = 1;
```
编译器将生成以下访问器方法：
- `bool has_foo() const`:

  如果字段已设置，则返回true。

- `const Bar& foo() const`:

  返回字段的当前值。如果未设置字段，则返回一个没有设置任何字段的Bar
  （可能是`Bar::default_instance`）。

- `Bar* mutable_foo()`:

  返回指向存储字段值的可变条对象的指针。如果在调用之前没有设置字段，那么返回的Bar将不会
  设置任何字段（即，它将与新分配的Bar相同）。调用此函数后，`has_foo()`将返回true，
  `foo()`将返回对同一个Bar实例的引用。

- `void clear_foo()`:

  清除字段的值。调用此函数后，`has_foo()`将返回false，`foo()`将返回默认值。

- `void set_allocated_foo(Bar* bar)`:

  将条形图对象设置为字段，并释放上一个字段值（如果存在）。如果Bar指针不为NULL，则消息将
  获得分配的Bar对象的所有权，`has_foo()`将返回true。否则，如果条为NULL，则行为与调用
  `clear_foo()`相同。

- `Bar* release_foo()`:

  释放字段的所有权并返回Bar对象的指针。调用此函数后，调用者获得分配的Bar对象的所有权，
  `has_foo()`将返回false，`foo()`将返回默认值。

#### 重复的数字类型

```protobuf
repeated int32 foo = 1;
```
编译器将生成以下访问器方法：
- `int foo_size() consts`:

  返回字段中当前的元素数。

- `int32 foo(int index) const`:

  返回给定的从零开始的索引处的元素。使用[0，`foo_size()`)之外的索引调用此方法会产生
  未定义的行为

- `void set_foo(int index, int32 value)`:

  在给定的从零开始的索引处设置元素的值。

- `void add_foo(int32 value)`:

  将新元素附加到具有给定值的字段末尾

- `void clear_foo()`:

  从字段中删除所有元素。调用此函数后，`foo_size()`将返回零

- `const RepeatedField<int32>& foo() const`:

  返回存储字段元素的基础RepeatedField。这个容器类提供类似STL的迭代器和其他方法

- `RepeatedField<int32>* mutable_foo()`:

  返回指向存储字段元素的基础可变RepeatedField的指针。这个容器类提供类似STL的迭代器和其他方法。

对于其他数值字段类型（包括bool），根据标量值类型表，用相应的C++的int32。

#### 重复的字符串字段

```protobuf
repeated string foo = 1;
repeated bytes foo = 1;
```
编译器将生成以下访问器方法：
- `int foo_size() const`:

  返回字段中当前的元素数。

- `const string& foo(int index) const`:

  返回给定的从零开始的索引处的元素。使用[0，`foo_size()`)之外的索引调用此方法会产生未定义的行为。

- `void set_foo(int index, const string& value)`:

  在给定的从零开始的索引处设置元素的值。s

- `void set_foo(int index, const char* value)`:

  使用C样式的以null结尾的字符串在给定的从零开始的索引处设置元素的值。

- `void set_foo(int index, const char* value, int size)`:

  如上所述，但字符串大小是显式给定的，而不是通过查找空终止符字节来确定的。

- `string* mutable_foo(int index)`:

  返回指向可变字符串对象的指针，该对象在给定的从零开始的索引处存储元素的值。使用
  [0，`foo_size()`)之外的索引调用此方法会产生未定义的行为。

- `void add_foo(const string& value)`:

  将新元素附加到具有给定值的字段末尾。

- `void add_foo(const char* value)`:

  使用C样式的以null结尾的字符串将新元素追加到字段末尾。

- `void add_foo(const char* value, int size)`:

  如上所述，但字符串大小是显式给定的，而不是通过查找空终止符字节来确定的。

- `string* add_foo()`:

  在字段末尾添加一个新的空字符串元素并返回指向它的指针。

- `void clear_foo()`:

  从字段中删除所有元素。调用此函数后，`foo_size()`将返回零。

- `const RepeatedPtrField<string>& foo() const`:

  返回存储字段元素的基础RepeatedPtrField。这个容器类提供类似STL的迭代器和其他方法。

- `RepeatedPtrField<string>* mutable_foo()`:

  返回指向存储字段元素的基础可变RepeatedPtrField的指针。这个容器类提供类似STL的迭代器
  和其他方法。

#### 重复枚举类型

```protobuf
enum Bar {
  BAR_VALUE = 0;
  OTHER_VALUE = 1;
}
```
定义:
```cpp
repeated Bar foo = 1;
```
编译器将生成以下访问器方法：
- `int foo_size() const`:v

  返回字段中当前的元素数。

- `Bar foo(int index) const`:

  返回给定的从零开始的索引处的元素。使用[0，`foo_size()`)之外的索引调用此方法会产生未
  定义的行为。

- `void set_foo(int index, Bar value)`:

  在给定的从零开始的索引处设置元素的值。在调试模式下（即未定义NDEBUG），如果值与为Bar
  定义的任何值不匹配，此方法将中止进程。

- `void add_foo(Bar value)`:

  将新元素附加到具有给定值的字段末尾。在调试模式下（即未定义NDEBUG），如果值与为Bar定义
  的任何值不匹配，此方法将中止进程。

- `void clear_foo()`:

  从字段中删除所有元素。调用此函数后，`foo_size()`将返回零。

- `const RepeatedField<int>& foo() const`:

  返回存储字段元素的基础RepeatedField。这个容器类提供类似STL的迭代器和其他方法。

- `RepeatedField<int>* mutable_foo()`:

  返回存储字段元素的基础RepeatedField。这个容器类提供类似STL的迭代器和其他方法。

#### 重复的嵌套类型

```protobuf
message Bar {}
```
字段定义:
```cpp
repeated Bar foo = 1;
```
编译器将生成以下访问器方法：
- `int foo_size() const`:

  返回字段中当前的元素数。

- `const Bar& foo(int index) const`:

  返回给定的从零开始的索引处的元素。使用[0，`foo_size()`)之外的索引调用此方法会产生未
  定义的行为。

- `Bar* mutable_foo(int index)`:

  返回指向可变条对象的指针，该对象在给定的从零开始的索引处存储元素的值。使用[0，`foo_size()`)
  之外的索引调用此方法会产生未定义的行为。

- `Bar* add_foo()`:

  在字段末尾添加新元素并返回指向它的指针。返回的条是可变的，并且不会设置任何字段（即，它
  将与新分配的条相同）。

- `void clear_foo()`:

  从字段中删除所有元素。调用此函数后，`foo_size()`将返回零。

- `const RepeatedPtrField<Bar>& foo() const`:

  返回存储字段元素的基础RepeatedPtrField。这个容器类提供类似STL的迭代器和其他方法。

- `RepeatedPtrField<Bar>* mutable_foo()`:

  返回指向存储字段元素的基础可变RepeatedPtrField的指针。这个容器类提供类似STL的迭代器
  和其他方法。

#### oneof数字字段

```protobuf
oneof oneof_name {
    int32 foo = 1;
    ...
}
```
编译器将生成以下访问器方法：
- `int32 foo() const`:

- `void set_foo(int32 value)`:

  - 如果设置了同一个字段中的任何其他字段，则调用`clear_oneof_name()`

  - 设置此字段的值，并将case的值设置为kFoo

- `void clear_foo()`:

  - 如果其中一个不是kFoo，什么都不会改变。

对于其他数值字段类型（包括布尔），根据[标量值类型表](https://developers.google.cn/protocol-buffers/docs/reference/language.shtml#scalar)，用相应的C++类型替换了int32

#### oneof字符串字段:

```protobuf
oneof oneof_name {
    string foo = 1;
    …
}
oneof oneof_name {
    bytes foo = 1;
    ….
}
```
编译器将生成以下访问器方法：
- `const string& foo() const`:

- `void set_foo(const string& value)`:

- `void set_foo(const char* value)`:

- `void set_foo(const char* value, int size)`:

- `string* mutable_foo()`:

- `void clear_foo()`:

- `void set_allocated_foo(string* value)`:

- `string* release_foo()`:

#### oneof枚举字段

```protobuf
enum Bar {
  BAR_VALUE = 0;
  OTHER_VALUE = 1;
}

oneof oneof_name {
    Bar foo = 1;
    ...
}
```
编译器将生成以下访问器方法：
- `Bar foo() const`:

- `void set_foo(Bar value)`:

- `void clear_foo()`:

#### oneof嵌套类型

```protobuf
message Bar {}

oneof oneof_name {
    Bar foo = 1;
    ...
}
```
编译器将生成以下访问器方法：
- `bool has_foo()`:

- `const Bar& foo() const`:

- `Bar* mutable_foo()`:

- `void clear_foo()`:

- `void set_allocated_foo(Bar* bar)`:

- `Bar* release_foo()`:

#### map字段

```protobuf
map<int32, int32> weight = 1;
```
编译器将生成以下访问器方法：
- `const google::protobuf::Map<int32, int32>& weight();`:

- `google::protobuf::Map<int32, int32>* mutable_weight();`:

`A google::protobuf::Map`是一种特殊的容器类型，用于协议缓冲区中存储Map字段。从下面的
接口可以看到，它使用了`std::map`和`std::unordered_map`方法的一个常用子集。
```cpp
template<typename Key, typename T> {
class Map {
  // Member types
  typedef Key key_type;
  typedef T mapped_type;
  typedef MapPair< Key, T > value_type;

  // Iterators
  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;
  // Capacity
  int size() const;
  bool empty() const;

  // Element access
  T& operator[](const Key& key);
  const T& at(const Key& key) const;
  T& at(const Key& key);

  // Lookup
  int count(const Key& key) const;
  const_iterator find(const Key& key) const;
  iterator find(const Key& key);

  // Modifiers
  pair<iterator, bool> insert(const value_type& value);
  template<class InputIt>
  void insert(InputIt first, InputIt last);
  size_type erase(const Key& Key);
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  void clear();

  // Copy
  Map(const Map& other);
  Map& operator=(const Map& other);
}
```
添加数据的最简单方法是使用普通映射语法，例如：
```cpp
std::unique_ptr<ProtoName> my_enclosing_proto(new ProtoName);
(*my_enclosing_proto->mutable_weight())[my_key] = my_value;
```
`pair<iterator, bool> insert(const value_type& value)`将隐式导致值类型实例的深度副本。
将新值插入`google::protobuf::Map`的最有效方法如下：
```cpp
T& operator[](const Key& key): map[new_key] = new_mapped;
```
在标准的map中使用`google::protobuf::Map`。

`google::protobuf::Map`支持`std::map`和`std::unordered_map`相同的迭代器api。
如果你不想直接使用`google::protobuf::Map`。你可以将`google::protobuf::Map`转换成标准
的map。
```cpp
std::map<int32, int32> standard_map(message.weight().begin(), message.weight().end());
```
请注意，这将生成整个map的深度副本。

您还可以从标准映射构造`google::protobuf::Map`映射，如下所示：
```cpp
google::protobuf::Map<int32, int32> weight(standard_map.begin(), standard_map.end());
```

解析未知字段

在协议缓冲区，.proto映射相当于每个键/值对的映射条目消息，而映射本身是映射条目的重复字段。
与普通消息类型一样，解析后的映射条目消息也可能具有未知字段：例如，定义为`map<int32，string>`的
映射中的int64类型字段。

如果map输入消息的协议缓冲区格式中存在未知字段，则将丢弃这些字段。

如果映射条目消息的协议缓冲区格式中存在未知枚举值，则在proto2和proto3中处理方式不同。
在proto2中，整个map entry消息被放入包含消息的未知字段集中。
在proto3中，它被放入map字段中，就好像它是一个已知的枚举值一样。

### any

```protobuf
import "google/protobuf/any.proto";

message ErrorStatus {
  string message = 1;
  google.protobuf.Any details = 2;
}
```
在我们生成的代码中，详细信息字段的getter返回`google::protobuf::Any`的实例。 这提供了
以下特殊方法来打包和解压缩Any的值：
```cpp
class Any {
 public:
  // Packs the given message into this Any using the default type URL
  // prefix “type.googleapis.com”. Returns false if serializing the message failed.
  bool PackFrom(const google::protobuf::Message& message);

  // Packs the given message into this Any using the given type URL
  // prefix. Returns false if serializing the message failed.
  bool PackFrom(const google::protobuf::Message& message,
                const string& type_url_prefix);

  // Unpacks this Any to a Message. Returns false if this Any
  // represents a different protobuf type or parsing fails.
  bool UnpackTo(google::protobuf::Message* message) const;

  // Returns true if this Any represents the given protobuf type.
  template<typename T> bool Is() const;
}
```

### oneof

```protobuf
oneof oneof_name {
    int32 foo_int = 4;
    string foo_string = 9;
    ...
}
```
编译器将生成以下C ++枚举类型：
```cpp
enum OneofNameCase {
  kFooInt = 4,
  kFooString = 9,
  ONEOF_NAME_NOT_SET = 0
}
```
此外，它将生成以下方法：
- `OneofNameCase oneof_name_case() const`:

  返回指示设置哪个字段的枚举。 如果均未设置，则返回`ONEOF_NAME_NOT_SET`。

- `void clear_oneof_name()`:

  如果oneof字段集使用指针（消息或字符串），则释放对象，并将oneof大小写设置为`ONEOF_NAME_NOT_SET`。

### 枚举

```protobuf
enum Foo {
  VALUE_A = 0;
  VALUE_B = 5;
  VALUE_C = 1234;
}
```
协议缓冲区编译器将生成具有相同值集的C++枚举类型，称为Foo。 此外，编译器还将生成以下函数：
- `const EnumDescriptor* Foo_descriptor()`:

  返回类型的描述符，其中包含有关此枚举类型定义的值的信息。

- `bool Foo_IsValid(int value)`:

  如果给定的数值与Foo的某个定义值匹配，则返回true。在上面的示例中，如果输入为0、5或1234，
  则返回true。

- `const string& Foo_Name(int value)`:

  返回给定数值的名称。如果不存在这样的值，则返回空字符串。如果多个值具有此数字，则返回定义
  的第一个值。在上面的例子中，`Foo_Name(5)`将返回`VALUE_B`。

- `bool Foo_Parse(const string& name, Foo* value)`:

  如果name是此枚举的有效值名称，则将该值赋给value并返回true。否则返回false。在上面的示
  例中，`Foo_Parse("VALUE_C", &some_foo)`将返回true并将`some_foo`设置为1234。

- `const Foo Foo_MIN`:

  枚举的最小有效值（示例中的`VALUE_A`）。

- `const Foo Foo_MAX`:

  枚举的最大有效值（示例中的`VALUE_C`）。

- `const int Foo_ARRAYSIZE`:

  总是定义成`Foo_MAX + 1`

在switch语句中使用proto3枚举时要小心。Proto3枚举是开放的枚举类型，可能的值超出指定符号
的范围。在解析proto3消息时，将保留无法识别的枚举值，并由枚举字段访问器返回。proto3枚举上
没有默认大小写的switch语句将无法捕获所有大小写，即使列出了所有已知字段。这可能导致意外行为，
包括数据损坏和运行时崩溃。始终在switch中添加`default`或显式调用`Foo_IsValid(int)`来处理
未知枚举值。

可以在消息类型中定义枚举。在本例中，协议缓冲区编译器生成的代码使枚举类型本身看起来嵌套在
消息的类中。`Foo_descriptor()`和`Foo_IsValid()`函数被声明为静态方法。实际上，枚举
类型本身及其值在全局范围中用损坏的名称声明，并用typedef和一系列常量定义导入类的范围。
这样做只是为了避开声明排序的问题。不要依赖损坏的顶级名称；假设枚举确实嵌套在消息类中。

### 扩展

proto3中不支持

### Arena Allocation

Arena allocation是C++上独有的特性。它可以帮助优化内存使用，并在使用协议缓冲区时提高性能。
在.proto文件中启用会在c++生成代码时添加额外的代码。见[C++ Arena Allocation Guide](https://developers.google.cn/protocol-buffers/docs/reference/arenas)

### 服务

如果.proto文件包含以下行：
```protobuf
option cc_generic_services = true;
```
然后，协议缓冲区编译器将根据文件中找到的服务定义生成代码，如本节所述。但是，生成的代码
可能是不需要的，因为它不绑定到任何特定的RPC系统，因此需要比为一个系统定制的代码更高级别
的间接寻址。如果不希望生成此代码，请将此行添加到文件中：
```protobuf
option cc_generic_services = false;
```
如果以上两行都没有给出，则该选项默认为false，因为通用服务已弃用。（请注意，在2.4.0之前，
该选项默认为true）

基于.proto语言服务定义的RPC系统应该提供插件来生成适合系统的代码。这些插件可能要求禁用抽象
服务，以便它们可以生成自己的同名类。插件是2.3.0版（2010年1月）的新版本。

- Interface

  ```protobuf
  service Foo {
    rpc Bar(FooRequest) returns(FooResponse);
  }
  ```
  协议缓冲区编译器将生成一个类Foo来表示这个服务。Foo将为服务定义中定义的每个方法提供一个
  虚拟方法。在这种情况下，方法栏定义为：
  ```cpp
  virtual void Bar(RpcController* controller, const FooRequest* request,
                 FooResponse* response, Closure* done);
  ```
  这些参数与`Service:：CallMethod()`的参数等价，只是方法参数是隐含的，请求和响应指定了
  它们的确切类型。

  这些生成的方法是虚拟的，但不是纯虚拟的。默认实现只需调用`controller->SetFailed()`，
  并显示一条错误消息，指示该方法未实现，然后调用done回调。在实现自己的服务时，必须将生成
  的服务子类化，并根据需要实现其方法。

  Foo是服务接口的子类。协议缓冲区编译器自动生成服务方法的实现，如下所示：
  - `GetDescriptor`:

    返回服务的`ServiceDescriptor`。

  - `CallMethod`:

    根据提供的方法描述符确定要调用哪个方法，并直接调用它，将请求和响应消息对象向下转换为正确的类型。

  - `GetRequestPrototype`和`GetResponsePrototype`:

    返回给定方法的正确类型的请求或响应的默认实例。

  还将生成以下静态方法：

  - `static ServiceDescriptor descriptor()`：

    返回类型的描述符，其中包含有关此服务有哪些方法及其输入和输出类型的信息。

- Stub

  协议缓冲区编译器还生成每个服务接口的`stub`实现，希望向实现该服务的服务器发送请求的客户
  端使用该实现。对于Foo服务（如上所述），将定义存根实现`Foo_Stub `。与嵌套的消息类型一样，
  使用typedef以便`Foo_Stub`也可以称为`Foo::Stub`。

  `Foo_Stub `是Foo的子类，它还实现以下方法：
  - `Foo_Stub(RpcChannel* channel)`:

    构造一个在给定通道上发送请求的新存根。

  - `Foo_Stub(RpcChannel* channel, ChannelOwnership ownership)`：

    构造一个新的存根，它在给定的通道上发送请求并可能拥有该通道。如果所有权是`Service::STUB_OWNS_CHANNEL `，
    那么当删除STUB对象时，它也会删除通道。

  - `RpcChannel* channel()`：

    返回传递给构造函数的这个存根的通道。

存根还将服务的每个方法作为通道的包装器来实现。
调用其中一个方法只需调用`channel->CallMethod()`

### 插件

代码生成器插件，要扩展C++代码生成器的输出，可以使用给定的插入点名称插入以下类型的代码。
每个插入点都出现在两个视图中`.pb.cc`文件和`.pb.h`文件，除非另有说明。
- `includes`:

  包含指令

- `namespace_scope`:

  属于文件的包/命名空间，但不属于任何特定类的声明。出现在所有其他命名空间作用域代码之后。

- `global_scope`:

  属于顶级、文件命名空间之外的声明。出现在文件的最后。

- `class_scope:TYPENAME`:

  属于消息类的成员声明。TYPENAME是完整的原名，例如。`package.MessageType`. 出现在类中
  所有其他公共声明之后。此插入点仅出现在`.pb.h`文件中。