# Typhoon Xlsx

本工具作为**Typhoon**数据解析工具。

![][p_xlsx2data]

- [Typhoon Xlsx](#typhoon-xlsx)
  - [前言](#前言)
  - [Demo](#demo)
  - [数据表格式要求](#数据表格式要求)
    - [导出表定义](#导出表定义)
    - [字段标签定义规则](#字段标签定义规则)
  - [使用说明](#使用说明)

## 前言

游戏开发中，策划配置数据经常会使用`excel`的的方式，所以开发此工具的目的在于将策划配置的
数据转化为程序中的数据文件。

## Demo

**Excel数据**
![][p_xlsx2data_excel]

**解析后生成**
![][p_xlsx2data_dir_json]

**c++ Adapter**
![][p_xlsx2data_cpp]

**proto**
文件未展示

可以参见 **demo** 目录

## 数据表格式要求

- 第一行为此列字段的标签定义，程序解析用。
- 第二行为此列字段的说明，策划阅读用。
- 第三行开始为表格数据。

### 导出表定义

- **@** 作为导出表名的前缀

  例如： `@Item`、`@Equip`、`Desc`中`Item`与`Equip`将作为有效表导出。

- 命名规范

  表名命名规则为**大驼峰**命名规范。 `item1`、`iItem`均为非良表名结构。

### 字段标签定义规则

- 字段组成方式

  **字段名**@**字段类型**@**字段约束**@**导出类型**@**字段检查**

  使用`@`分割字段定义，顺序不可以颠倒，排在后面的定义可以为空，但是中间不可以为空。

- 字段名

  - 命名规范

    采用**google**命名规范 https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#variable-names

- 字段类型

  因为要转换为`json`与`protobuf`，所以类型有所取舍。

  支持类型如下：
  |   xlsx    |  json  | protobuf |
  | :-------: | :----: | :------: |
  |  double   | double |  double  |
  |   float   | double |  float   |
  |    i32    |  int   |  int32   |
  |    i64    | int64  |  int64   |
  |    u32    |  uint  |  uint32  |
  |    u64    | unt64  |  uint64  |
  |   bool    |  bool  |   bool   |
  |    str    | string |  string  |
  | desc or # |        |          |
  |  complex  |        |          |
  
  其中`#`为描述内容，该列数据不会被解析。

  其中`complex`为复合类型。具体解析分情况。

  - 复合对象

    - 字段名 **u32-u32**
    - 例子 **1001-800**

  - 复合数组

    - 字段名 **u32-u32|**
    - 例子 **1001-800|1002-900**

  - 复合嵌套数组

    - 字段名 **u32-u32;|**
    - 例子 **1001-800;1002-300|2001-800;1002-500;2003-600**

- 字段约束

  - `*`

    该字段为必填项。检查数据时，会检查非空。

  - `!`

    主键标识，如果同表中存在多个主键，按照先后顺序拼接。要求主键集合唯一。检查数据时，
    会检查唯一性。主键一定是非空的，所以自带`*`属性。

- 导出类型

  - c   仅客户端可用
  - s   仅服务器可用
  - cs  默认 服务器 客户端都可用 不填写默认CS

- 字段检查

  `表名.字段名`的组合，数据导出后，会对带有字段检查的字段进行检查。

  如果是复合类型中的字段检查，需要和复合类型的描述一致，无需检查的字段使用`$`站位。


**Demo**
| id@i32@! | name@str@\*@c | type@i32@\* | log_type@i32@@s@Log.type |  desc@#   |
| :------: | :-----------: | :---------: | :----------------------: | :-------: |
|   1001   |     道具1     |      1      |            5             | 这是道具1 |
|   1002   |     item2     |      2      |                          | 这是item2 |

- `id@i32@!`

  | 字段名 | 字段类型 |  字段约束  | 导出类型 | 字段检查 |
  | :----: | :------: | :--------: | :------: | :------: |
  |   id   |   i32    | ! 主键非空 |    cs    |          |

- `name@str@*@c`

  | 字段名 | 字段类型 | 字段约束 | 导出类型 | 字段检查 |
  | :----: | :------: | :------: | :------: | :------: |
  |  name  |   str    |  * 非空  | c 客户端 |          |

- `type@i32@*`

  | 字段名 | 字段类型 | 字段约束 | 导出类型 | 字段检查 |
  | :----: | :------: | :------: | :------: | :------: |
  |  type  |   i32    |  * 非空  |    cs    |          |

- `log_type@i32@@s@Log.type`

  |  字段名  | 字段类型 | 字段约束 | 导出类型 |        字段检查        |
  | :------: | :------: | :------: | :------: | :--------------------: |
  | log_type |   i32    |          | s 服务器 | 检查 Log表中的type字段 |

- `desc@#`

  | 字段名 | 字段类型 | 字段约束 | 导出类型 | 字段检查 |
  | :----: | :------: | :------: | :------: | :------: |
  |  desc  |    #     |          |          |          |

## 使用说明

先说明一下文件夹以及文件的用途。下列文件夹及文件都是`xlsx2data`文件夹的子目录。

- bin

  protobuf 二进制文件，不会直接生成。需要第二次编译才会出现。

  `data_hub.bin`是data所有数据的二进制文件，可以反序列化为程序需要的文件。

- cpp

  cpp的adapter文件。

  `data_hub.h`与`data_hub.cpp`这两个文件可以操作反序列化后的文件。

  如果excel文件新增或者更改结构。这两个文件需要拷贝到`tpn/src/lib/data`下，并且重新编译。

- data

  存放要解析的excel文件的目录，可以手动拷贝。只会解析`.xslx`后缀的文件。

- generator

  json文件转换为bin文件的生成器。如果有修改需要替换掉`tpn_xlsx/generator`目录下的文件。

- json

  excel文件转换后的json文件。这种json文件是特殊的文件。可以被protobuf转换的文件。

- log

  日志文件

- proto

  protobuf描述文件，根据excel动态生成。

1.  初始项目编译完成后，需要切换到生成目录。与`protoc`同级的目录。
2.  执行命令
    ```shell
    ./protoc --cpp_out="./xlsx2data/proto" --proto_path="./xlsx2data/proto" data_hub.proto
    ```
    你可能会遇到类似问题
    ```shell
    google/protobuf/any.proto: File not found.
    data_hub.proto:25:1: Import "google/protobuf/any.proto" was not found or had errors.
    data_hub.proto: "google.protobuf.Any" is not defined.
    ```
    只需要将`tools/proto`目录下的`google`目录复制到`data_hub.proto`所在目录即可。再次
    执行命令。
3.  经过步骤2，与`data_hub.proto`同级的目录中应该生成了`data_hub.pb.h`与`data_hub.pb.cc`两个文件。
4.  `data_hub.pb.h`与`data_hub.pb.cc`需要拷贝到`tpn_xlsx/pb`以及`tpn/src/lib/data/pb`目录下，并重新编译。

<!-- image -->
[p_xlsx2data]:            ./doc/xlsx2data.png
[p_xlsx2data_excel]:      ./doc/xlsx2data_excel.png
[p_xlsx2data_dir_json]:   ./doc/xlsx2data_dir_json.png
[p_xlsx2data_cpp]:        ./doc/xlsx2data_cpp.png

[p_xlsx2data_cpp_head]:   ./doc/xlsx2data_cpp_head.png
[p_xlsx2data_cpp_source]: ./doc/xlsx2data_cpp_source.png
[p_xlsx2data_dir]:        ./doc/xlsx2data_dir.png

[p_xlsx2data_json_item]:  ./doc/xlsx2data_json_item.png
[p_xlsx2data_json_level]: ./doc/xlsx2data_json_level.png
[p_xlsx2data_json_pack]:  ./doc/xlsx2data_json_pack.png
[p_xlsx2data_json_shop]:  ./doc/xlsx2data_json_shop.png
[p_xlsx2data_json_skill]: ./doc/xlsx2data_json_skill.png
