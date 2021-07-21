# Typhoon Xlsx

本工具作为**Typhoon**数据解析工具。

## 前言

游戏开发中，策划配置数据经常会使用`excel`的的方式，所以开发此工具的目的在于将策划配置的
数据转化为程序中的数据文件。

## 数据表格式要求

- 第一行为此列字段的标签定义，程序解析用
- 第二行为此列字段的说明，策划阅读用
- 第三行开始为表格数据

**Demo**
| id[]  | name[] | type[] |
| :---: | :----: | :----: |
| 1001  | 道具1  |   1    |
| 1002  | item2  |   2    |

上述表格是不可以使用的，完整解析见下方。

### 字段标签定义规则

google命名规范 https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#variable-names

```markdown
字段名(必须满足google命名规范) 例如: first_name、last_name。
```