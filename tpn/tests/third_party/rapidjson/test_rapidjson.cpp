//
//           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
//            │ └┬┘├─┘├─┤│ ││ ││││
//            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
//
// This file is part of the typhoon Project.
// Copyright (C) 2021 stanley0207@163.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "../../test_include.h"

#include <rapidjson/document.h>      // rapidjson's DOM-style API
#include <rapidjson/prettywriter.h>  // for stringify JSON

#include <cstdio>

using namespace rapidjson;
using namespace std;

TEST_CASE("demo", "[rapidjson]") {
  ////////////////////////////////////////////////////////////////////////////
  // 1. Parse a JSON text string to a document.

  const char json[] =
      " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, "
      "\"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
  printf("Original JSON:\n %s\n", json);

  Document document;  // Default template parameter uses UTF8 and
                      // MemoryPoolAllocator.

#if 0
    // "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
    if (document.Parse(json).HasParseError())
        return;
#else
  // In-situ parsing, decode strings directly in the source string. Source must
  // be string.
  char buffer[sizeof(json)];
  memcpy(buffer, json, sizeof(json));
  if (document.ParseInsitu(buffer).HasParseError()) return;
#endif

  printf("\nParsing to document succeeded.\n");

  ////////////////////////////////////////////////////////////////////////////
  // 2. Access values in document.

  printf("\nAccess values in document:\n");
  assert(
      document.IsObject());  // Document is a JSON value represents the root of
                             // DOM. Root can be either an object or array.

  assert(document.HasMember("hello"));
  assert(document["hello"].IsString());
  printf("hello = %s\n", document["hello"].GetString());

  // Since version 0.2, you can use single lookup to check the existing of
  // member and its value:
  Value::MemberIterator hello = document.FindMember("hello");
  assert(hello != document.MemberEnd());
  assert(hello->value.IsString());
  assert(strcmp("world", hello->value.GetString()) == 0);
  (void)hello;

  assert(document["t"].IsBool());  // JSON true/false are bool. Can also uses
                                   // more specific function IsTrue().
  printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

  assert(document["f"].IsBool());
  printf("f = %s\n", document["f"].GetBool() ? "true" : "false");

  printf("n = %s\n", document["n"].IsNull() ? "null" : "?");

  assert(document["i"].IsNumber());  // Number is a JSON type, but C++ needs
                                     // more specific type.
  assert(document["i"].IsInt());  // In this case, IsUint()/IsInt64()/IsUint64()
                                  // also return true.
  printf("i = %d\n", document["i"].GetInt());  // Alternative (int)document["i"]

  assert(document["pi"].IsNumber());
  assert(document["pi"].IsDouble());
  printf("pi = %g\n", document["pi"].GetDouble());

  {
    const Value &a = document["a"];  // Using a reference for consecutive access
                                     // is handy and faster.
    assert(a.IsArray());
    for (SizeType i = 0; i < a.Size();
         i++)  // rapidjson uses SizeType instead of size_t.
      printf("a[%d] = %d\n", i, a[i].GetInt());

    int y = a[0].GetInt();
    (void)y;

    // Iterating array with iterators
    printf("a = ");
    for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
      printf("%d ", itr->GetInt());
    printf("\n");
  }

  // Iterating object members
  static const char *kTypeNames[] = {"Null",  "False",  "True",  "Object",
                                     "Array", "String", "Number"};
  for (Value::ConstMemberIterator itr = document.MemberBegin();
       itr != document.MemberEnd(); ++itr)
    printf("Type of member %s is %s\n", itr->name.GetString(),
           kTypeNames[itr->value.GetType()]);

  ////////////////////////////////////////////////////////////////////////////
  // 3. Modify values in document.

  // Change i to a bigger number
  {
    uint64_t f20 = 1;  // compute factorial of 20
    for (uint64_t j = 1; j <= 20; j++) f20 *= j;
    document["i"] = f20;  // Alternate form: document["i"].SetUint64(f20)
    assert(!document["i"].IsInt());  // No longer can be cast as int or uint.
  }

  // Adding values to array.
  {
    Value &a = document["a"];  // This time we uses non-const reference.
    Document::AllocatorType &allocator = document.GetAllocator();
    for (int i = 5; i <= 10; i++)
      a.PushBack(i,
                 allocator);  // May look a bit strange, allocator is needed for
    // potentially realloc. We normally uses the document's.

    // Fluent API
    a.PushBack("Lua", allocator).PushBack("Mio", allocator);
  }

  // Making string values.

  // This version of SetString() just store the pointer to the string.
  // So it is for literal and string that exists within value's life-cycle.
  {
    document["hello"] = "rapidjson";  // This will invoke strlen()
                                      // Faster version:
    // document["hello"].SetString("rapidjson", 9);
  }

  // This version of SetString() needs an allocator, which means it will
  // allocate a new buffer and copy the the string into the buffer.
  Value author;
  {
    char buffer2[10];
    int len =
        sprintf(buffer2, "%s %s", "Milo",
                "Yip");  // synthetic example of dynamically created string.

    author.SetString(buffer2, static_cast<SizeType>(len),
                     document.GetAllocator());
    // Shorter but slower version:
    // document["hello"].SetString(buffer, document.GetAllocator());

    // Constructor version:
    // Value author(buffer, len, document.GetAllocator());
    // Value author(buffer, document.GetAllocator());
    memset(buffer2, 0, sizeof(buffer2));  // For demonstration purpose.
  }
  // Variable 'buffer' is unusable now but 'author' has already made a copy.
  document.AddMember("author", author, document.GetAllocator());

  assert(
      author.IsNull());  // Move semantic for assignment. After this variable is
                         // assigned as a member, the variable becomes null.

  ////////////////////////////////////////////////////////////////////////////
  // 4. Stringify JSON

  printf("\nModified JSON with reformatting:\n");
  StringBuffer sb;
  PrettyWriter<StringBuffer> writer(sb);
  document.Accept(
      writer);  // Accept() traverses the DOM and generates Handler events.
  puts(sb.GetString());
}

TEST_CASE("one_line", "[rapidjson]") {
  const char json[] =
      "{\"header\":{\"stamp\":14321212,\"frame_id\":\"base_lasr "
      "12321\"},\"mapInfo\":{\"gridWidth\":1000,\"gridHeight\":1000,"
      "\"originX\":-41.273852042959405,\"originY\":-39.639522665878005,"
      "\"resolution\":0.05},\"gridPhits\":[{\"x\":111,\"y\":111},{\"x\":111,"
      "\"y\":111},{\"x\":111,\"y\":111}],\"test_double\":-4.000003518013518e-"
      "12,\"test_double2\":-3.5180135178e-05,\"test_double3\":3.035178,"
      "\"worldPhits\":[{\"x\":-4.000003518013518e-12,\"y\":2.000000003244042e-"
      "06,\"z\":0},{\"x\":-6.546180135178e-13,\"y\":4.3546042026e-14,\"z\":0},{"
      "\"x\":-2.1380135178e-09,\"y\":4.2184042026e-12,\"z\":0}]}";
  printf("Original JSON:\n %s\n", json);

  Document document;
  char buffer[sizeof(json)];
  memcpy(buffer, json, sizeof(json));
  if (document.ParseInsitu(buffer).HasParseError()) {
    return;
  }

  printf("\nParsing to document succeeded.\n");

  printf("\nAccess values in document:\n");
  assert(
      document.IsObject());  // Document is a JSON value represents the root of
                             // DOM. Root can be either an object or array.

  assert(document["test_double"].IsNumber());
  assert(document["test_double"].IsDouble());
  printf("pi = %g\n", document["test_double"].GetDouble());
  printf("pi = %g\n", document["test_double2"].GetDouble());
  printf("pi = %g\n", document["test_double3"].GetDouble());
}

TEST_CASE("comment", "[rapidjson]") {
  const char *json =
      "// Here is a one-line comment.\n"
      "{// And here's another one\n"
      "   /*And here's an in-line one.*/\"hello\" : \"world\","
      "   \"t\" :/* And one with '*' symbol*/true ,"
      "/* A multiline comment\n"
      "   goes here*/"
      "   \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, "
      "3]"
      "}/*And the last one to be sure */";

  printf("\nComment JSON:\n %s\n", json);

  Document document;
  StringStream s(json);
  if (document.ParseStream<kParseCommentsFlag>(s).HasParseError()) {
    printf("pass error\n");
    return;
  }

  printf("\nParsing to document succeeded.\n");

  ////////////////////////////////////////////////////////////////////////////
  // 2. Access values in document.

  printf("\nAccess values in document:\n");
  assert(
      document.IsObject());  // Document is a JSON value represents the root of
                             // DOM. Root can be either an object or array.

  assert(document.HasMember("hello"));
  assert(document["hello"].IsString());
  printf("hello = %s\n", document["hello"].GetString());

  // Since version 0.2, you can use single lookup to check the existing of
  // member and its value:
  Value::MemberIterator hello = document.FindMember("hello");
  assert(hello != document.MemberEnd());
  assert(hello->value.IsString());
  assert(strcmp("world", hello->value.GetString()) == 0);
  (void)hello;

  assert(document["t"].IsBool());  // JSON true/false are bool. Can also uses
                                   // more specific function IsTrue().
  printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

  assert(document["f"].IsBool());
  printf("f = %s\n", document["f"].GetBool() ? "true" : "false");

  printf("n = %s\n", document["n"].IsNull() ? "null" : "?");

  assert(document["i"].IsNumber());  // Number is a JSON type, but C++ needs
                                     // more specific type.
  assert(document["i"].IsInt());  // In this case, IsUint()/IsInt64()/IsUint64()
                                  // also return true.
  printf("i = %d\n", document["i"].GetInt());  // Alternative (int)document["i"]

  assert(document["pi"].IsNumber());
  assert(document["pi"].IsDouble());
  printf("pi = %g\n", document["pi"].GetDouble());

  {
    const Value &a = document["a"];  // Using a reference for consecutive access
                                     // is handy and faster.
    assert(a.IsArray());
    for (SizeType i = 0; i < a.Size();
         i++)  // rapidjson uses SizeType instead of size_t.
      printf("a[%d] = %d\n", i, a[i].GetInt());

    int y = a[0].GetInt();
    (void)y;

    // Iterating array with iterators
    printf("a = ");
    for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
      printf("%d ", itr->GetInt());
    printf("\n");
  }
}

#include <filesystem>

#include <rapidjson/filereadstream.h>

TEST_CASE("file", "[rapidjson]") {
  printf("\n");
#ifndef _TPN_RAPIDJSON_TEST_FILE
#  define _TPN_RAPIDJSON_TEST_FILE "configtest.json"
#endif
  namespace fs = std::filesystem;
  // const auto json_path = fs::path(_TPN_RAPIDJSON_TEST_FILE);
  // auto json_file       = fs::absolute(json_path);
  auto json_file = fs::absolute(_TPN_RAPIDJSON_TEST_FILE);
#ifdef _WIN32
  FILE *fp = fopen(json_file.generic_string().c_str(), "rb");
#else
  FILE *fp = fopen(json_file.generic_string().c_str(), "r");
#endif
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));

  Document d;
  d.ParseStream<kParseCommentsFlag>(is);

  if (d.HasParseError()) {
    printf("file parse.");
    return;
  }

  if (!d.HasMember("loglevel")) {
    printf("loglevel not has\n");
  }

  if (!d.HasMember("logleveltttt")) {
    printf("loglevelttt not has\n");
  }

  if (d["loglevel"].IsInt()) {
    printf("loglevel   %d\n", d["loglevel"].GetInt());
  }

  if (!d.HasMember("host")) {
    printf("host not has\n");
  }

  if (d["host"].IsString()) {
    printf("host %s\n", d["host"].GetString());
  }

  if (d.HasMember("mapInfo")) {
    printf("has mapInfo\n");

    if (d["mapInfo"].IsObject()) {
      printf("mapInfo is object\n");
    }
  }

  fclose(fp);
}

// object
TEST_CASE("object", "[rapidjson]") {
  printf("\n");
#ifndef _TPN_RAPIDJSON_TEST_FILE
#  define _TPN_RAPIDJSON_TEST_FILE "configtest.json"
#endif
  namespace fs = std::filesystem;
  // const auto json_path = fs::path(_TPN_RAPIDJSON_TEST_FILE);
  // auto json_file       = fs::absolute(json_path);
  auto json_file = fs::absolute(_TPN_RAPIDJSON_TEST_FILE);
#ifdef _WIN32
  FILE *fp = fopen(json_file.generic_string().c_str(), "rb");
#else
  FILE *fp = fopen(json_file.generic_string().c_str(), "r");
#endif
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));

  Document d;
  d.ParseStream<kParseCommentsFlag>(is);

  if (d.HasParseError()) {
    printf("file parse.");
    return;
  }

  if (d.HasMember("mapInfo")) {
    printf("has mapInfo\n");

    if (d["mapInfo"].IsObject()) {
      printf("mapInfo is object\n");
    }

    for (auto &&[name, value] : d["mapInfo"].GetObject()) {
      printf("%s %lf\n", name.GetString(), value.GetDouble());
    }
  }

  fclose(fp);
}
