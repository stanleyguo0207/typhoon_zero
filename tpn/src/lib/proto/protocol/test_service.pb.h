// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocol/test_service.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_protocol_2ftest_5fservice_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_protocol_2ftest_5fservice_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/service.h>
#include <google/protobuf/unknown_field_set.h>
#include "type/rpc_type.pb.h"
#include "service_base.h"
#include "byte_buffer.h"
#include <functional>
#include <type_traits>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_protocol_2ftest_5fservice_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_protocol_2ftest_5fservice_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_protocol_2ftest_5fservice_2eproto;
namespace tpn {
namespace protocol {
class Result;
struct ResultDefaultTypeInternal;
extern ResultDefaultTypeInternal _Result_default_instance_;
class SearchRequest;
struct SearchRequestDefaultTypeInternal;
extern SearchRequestDefaultTypeInternal _SearchRequest_default_instance_;
class SearchResponse;
struct SearchResponseDefaultTypeInternal;
extern SearchResponseDefaultTypeInternal _SearchResponse_default_instance_;
}  // namespace protocol
}  // namespace tpn
PROTOBUF_NAMESPACE_OPEN
template<> ::tpn::protocol::Result* Arena::CreateMaybeMessage<::tpn::protocol::Result>(Arena*);
template<> ::tpn::protocol::SearchRequest* Arena::CreateMaybeMessage<::tpn::protocol::SearchRequest>(Arena*);
template<> ::tpn::protocol::SearchResponse* Arena::CreateMaybeMessage<::tpn::protocol::SearchResponse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace tpn {
namespace protocol {

// ===================================================================

class SearchRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:tpn.protocol.SearchRequest) */ {
 public:
  inline SearchRequest() : SearchRequest(nullptr) {}
  ~SearchRequest() override;
  explicit constexpr SearchRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SearchRequest(const SearchRequest& from);
  SearchRequest(SearchRequest&& from) noexcept
    : SearchRequest() {
    *this = ::std::move(from);
  }

  inline SearchRequest& operator=(const SearchRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline SearchRequest& operator=(SearchRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const SearchRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const SearchRequest* internal_default_instance() {
    return reinterpret_cast<const SearchRequest*>(
               &_SearchRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SearchRequest& a, SearchRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(SearchRequest* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SearchRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SearchRequest* New() const final {
    return new SearchRequest();
  }

  SearchRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SearchRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const SearchRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const SearchRequest& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SearchRequest* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "tpn.protocol.SearchRequest";
  }
  protected:
  explicit SearchRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kQueryFieldNumber = 1,
    kPageNumberFieldNumber = 2,
    kResultPerPageFieldNumber = 3,
  };
  // string query = 1;
  void clear_query();
  const std::string& query() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_query(ArgT0&& arg0, ArgT... args);
  std::string* mutable_query();
  PROTOBUF_MUST_USE_RESULT std::string* release_query();
  void set_allocated_query(std::string* query);
  private:
  const std::string& _internal_query() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_query(const std::string& value);
  std::string* _internal_mutable_query();
  public:

  // int32 page_number = 2;
  void clear_page_number();
  ::PROTOBUF_NAMESPACE_ID::int32 page_number() const;
  void set_page_number(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_page_number() const;
  void _internal_set_page_number(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 result_per_page = 3;
  void clear_result_per_page();
  ::PROTOBUF_NAMESPACE_ID::int32 result_per_page() const;
  void set_result_per_page(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_result_per_page() const;
  void _internal_set_result_per_page(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:tpn.protocol.SearchRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr query_;
  ::PROTOBUF_NAMESPACE_ID::int32 page_number_;
  ::PROTOBUF_NAMESPACE_ID::int32 result_per_page_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_protocol_2ftest_5fservice_2eproto;
};
// -------------------------------------------------------------------

class SearchResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:tpn.protocol.SearchResponse) */ {
 public:
  inline SearchResponse() : SearchResponse(nullptr) {}
  ~SearchResponse() override;
  explicit constexpr SearchResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  SearchResponse(const SearchResponse& from);
  SearchResponse(SearchResponse&& from) noexcept
    : SearchResponse() {
    *this = ::std::move(from);
  }

  inline SearchResponse& operator=(const SearchResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline SearchResponse& operator=(SearchResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const SearchResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const SearchResponse* internal_default_instance() {
    return reinterpret_cast<const SearchResponse*>(
               &_SearchResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(SearchResponse& a, SearchResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(SearchResponse* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SearchResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SearchResponse* New() const final {
    return new SearchResponse();
  }

  SearchResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SearchResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const SearchResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const SearchResponse& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SearchResponse* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "tpn.protocol.SearchResponse";
  }
  protected:
  explicit SearchResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kResultsFieldNumber = 1,
  };
  // repeated .tpn.protocol.Result results = 1;
  int results_size() const;
  private:
  int _internal_results_size() const;
  public:
  void clear_results();
  ::tpn::protocol::Result* mutable_results(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::tpn::protocol::Result >*
      mutable_results();
  private:
  const ::tpn::protocol::Result& _internal_results(int index) const;
  ::tpn::protocol::Result* _internal_add_results();
  public:
  const ::tpn::protocol::Result& results(int index) const;
  ::tpn::protocol::Result* add_results();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::tpn::protocol::Result >&
      results() const;

  // @@protoc_insertion_point(class_scope:tpn.protocol.SearchResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::tpn::protocol::Result > results_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_protocol_2ftest_5fservice_2eproto;
};
// -------------------------------------------------------------------

class Result final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:tpn.protocol.Result) */ {
 public:
  inline Result() : Result(nullptr) {}
  ~Result() override;
  explicit constexpr Result(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Result(const Result& from);
  Result(Result&& from) noexcept
    : Result() {
    *this = ::std::move(from);
  }

  inline Result& operator=(const Result& from) {
    CopyFrom(from);
    return *this;
  }
  inline Result& operator=(Result&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Result& default_instance() {
    return *internal_default_instance();
  }
  static inline const Result* internal_default_instance() {
    return reinterpret_cast<const Result*>(
               &_Result_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(Result& a, Result& b) {
    a.Swap(&b);
  }
  inline void Swap(Result* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Result* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Result* New() const final {
    return new Result();
  }

  Result* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Result>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Result& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const Result& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to, const ::PROTOBUF_NAMESPACE_ID::Message&from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Result* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "tpn.protocol.Result";
  }
  protected:
  explicit Result(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSnippetsFieldNumber = 3,
    kUrlFieldNumber = 1,
    kTitleFieldNumber = 2,
  };
  // repeated string snippets = 3;
  int snippets_size() const;
  private:
  int _internal_snippets_size() const;
  public:
  void clear_snippets();
  const std::string& snippets(int index) const;
  std::string* mutable_snippets(int index);
  void set_snippets(int index, const std::string& value);
  void set_snippets(int index, std::string&& value);
  void set_snippets(int index, const char* value);
  void set_snippets(int index, const char* value, size_t size);
  std::string* add_snippets();
  void add_snippets(const std::string& value);
  void add_snippets(std::string&& value);
  void add_snippets(const char* value);
  void add_snippets(const char* value, size_t size);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>& snippets() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>* mutable_snippets();
  private:
  const std::string& _internal_snippets(int index) const;
  std::string* _internal_add_snippets();
  public:

  // string url = 1;
  void clear_url();
  const std::string& url() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_url(ArgT0&& arg0, ArgT... args);
  std::string* mutable_url();
  PROTOBUF_MUST_USE_RESULT std::string* release_url();
  void set_allocated_url(std::string* url);
  private:
  const std::string& _internal_url() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_url(const std::string& value);
  std::string* _internal_mutable_url();
  public:

  // string title = 2;
  void clear_title();
  const std::string& title() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_title(ArgT0&& arg0, ArgT... args);
  std::string* mutable_title();
  PROTOBUF_MUST_USE_RESULT std::string* release_title();
  void set_allocated_title(std::string* title);
  private:
  const std::string& _internal_title() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_title(const std::string& value);
  std::string* _internal_mutable_title();
  public:

  // @@protoc_insertion_point(class_scope:tpn.protocol.Result)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string> snippets_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr url_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr title_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_protocol_2ftest_5fservice_2eproto;
};
// ===================================================================

class TestService1 : public ServiceBase {
 public:

  explicit TestService1(bool use_original_hash);
  virtual ~TestService1();

  using OriginalHash = std::integral_constant<uint32_t, 0x63F1143Fu>;
  using NameHash = std::integral_constant<uint32_t, 0x1A48BF3Fu>;

  static const google::protobuf::ServiceDescriptor *descriptor();

  // client methods --------------------------------------------------
  void ProcessClientRequest11(const ::tpn::protocol::SearchRequest *request, bool client = false, bool server = false);
  void ProcessClientRequest12(const ::tpn::protocol::SearchRequest *request, std::function<void(const ::tpn::protocol::SearchResponse *)> response_callback, bool client = false, bool server = false);

  void CallServerMethod(uint32_t token, uint32_t method_id, ByteBuffer buffer) final;
 private:
  uint32_t service_hash_{0};

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TestService1);
};

// -------------------------------------------------------------------

class TestService2 : public ServiceBase {
 public:

  explicit TestService2(bool use_original_hash);
  virtual ~TestService2();

  using OriginalHash = std::integral_constant<uint32_t, 0x52265835u>;
  using NameHash = std::integral_constant<uint32_t, 0x1B48C0D2u>;

  static const google::protobuf::ServiceDescriptor *descriptor();

  void CallServerMethod(uint32_t token, uint32_t method_id, ByteBuffer buffer) final;
 protected:
  // server methods --------------------------------------------------
  virtual uint32_t HandleProcessClientRequest21(const ::tpn::protocol::SearchRequest *request);
  virtual uint32_t HandleProcessClientRequest22(const ::tpn::protocol::SearchRequest *request, ::tpn::protocol::SearchResponse *response, std::function<void(ServiceBase*, uint32_t, const google::protobuf::Message *)> &continuation);

 private:
  uint32_t service_hash_{0};

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TestService2);
};

// -------------------------------------------------------------------

class TestService3 : public ServiceBase {
 public:

  explicit TestService3(bool use_original_hash);
  virtual ~TestService3();

  using OriginalHash = std::integral_constant<uint32_t, 0x512656A2u>;
  using NameHash = std::integral_constant<uint32_t, 0x1C48C265u>;

  static const google::protobuf::ServiceDescriptor *descriptor();

  // client methods --------------------------------------------------
  void ProcessClientRequest31(const ::tpn::protocol::SearchRequest *request, bool client = false, bool server = false);
  void ProcessClientRequest32(const ::tpn::protocol::SearchRequest *request, std::function<void(const ::tpn::protocol::SearchResponse *)> response_callback, bool client = false, bool server = false);

  void CallServerMethod(uint32_t token, uint32_t method_id, ByteBuffer buffer) final;
 protected:
  // server methods --------------------------------------------------
  virtual uint32_t HandleProcessClientRequest31(const ::tpn::protocol::SearchRequest *request);
  virtual uint32_t HandleProcessClientRequest32(const ::tpn::protocol::SearchRequest *request, ::tpn::protocol::SearchResponse *response, std::function<void(ServiceBase*, uint32_t, const google::protobuf::Message *)> &continuation);

 private:
  uint32_t service_hash_{0};

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(TestService3);
};

// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SearchRequest

// string query = 1;
inline void SearchRequest::clear_query() {
  query_.ClearToEmpty();
}
inline const std::string& SearchRequest::query() const {
  // @@protoc_insertion_point(field_get:tpn.protocol.SearchRequest.query)
  return _internal_query();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void SearchRequest::set_query(ArgT0&& arg0, ArgT... args) {
 
 query_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:tpn.protocol.SearchRequest.query)
}
inline std::string* SearchRequest::mutable_query() {
  std::string* _s = _internal_mutable_query();
  // @@protoc_insertion_point(field_mutable:tpn.protocol.SearchRequest.query)
  return _s;
}
inline const std::string& SearchRequest::_internal_query() const {
  return query_.Get();
}
inline void SearchRequest::_internal_set_query(const std::string& value) {
  
  query_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* SearchRequest::_internal_mutable_query() {
  
  return query_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* SearchRequest::release_query() {
  // @@protoc_insertion_point(field_release:tpn.protocol.SearchRequest.query)
  return query_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void SearchRequest::set_allocated_query(std::string* query) {
  if (query != nullptr) {
    
  } else {
    
  }
  query_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), query,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:tpn.protocol.SearchRequest.query)
}

// int32 page_number = 2;
inline void SearchRequest::clear_page_number() {
  page_number_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 SearchRequest::_internal_page_number() const {
  return page_number_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 SearchRequest::page_number() const {
  // @@protoc_insertion_point(field_get:tpn.protocol.SearchRequest.page_number)
  return _internal_page_number();
}
inline void SearchRequest::_internal_set_page_number(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  page_number_ = value;
}
inline void SearchRequest::set_page_number(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_page_number(value);
  // @@protoc_insertion_point(field_set:tpn.protocol.SearchRequest.page_number)
}

// int32 result_per_page = 3;
inline void SearchRequest::clear_result_per_page() {
  result_per_page_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 SearchRequest::_internal_result_per_page() const {
  return result_per_page_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 SearchRequest::result_per_page() const {
  // @@protoc_insertion_point(field_get:tpn.protocol.SearchRequest.result_per_page)
  return _internal_result_per_page();
}
inline void SearchRequest::_internal_set_result_per_page(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  result_per_page_ = value;
}
inline void SearchRequest::set_result_per_page(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_result_per_page(value);
  // @@protoc_insertion_point(field_set:tpn.protocol.SearchRequest.result_per_page)
}

// -------------------------------------------------------------------

// SearchResponse

// repeated .tpn.protocol.Result results = 1;
inline int SearchResponse::_internal_results_size() const {
  return results_.size();
}
inline int SearchResponse::results_size() const {
  return _internal_results_size();
}
inline void SearchResponse::clear_results() {
  results_.Clear();
}
inline ::tpn::protocol::Result* SearchResponse::mutable_results(int index) {
  // @@protoc_insertion_point(field_mutable:tpn.protocol.SearchResponse.results)
  return results_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::tpn::protocol::Result >*
SearchResponse::mutable_results() {
  // @@protoc_insertion_point(field_mutable_list:tpn.protocol.SearchResponse.results)
  return &results_;
}
inline const ::tpn::protocol::Result& SearchResponse::_internal_results(int index) const {
  return results_.Get(index);
}
inline const ::tpn::protocol::Result& SearchResponse::results(int index) const {
  // @@protoc_insertion_point(field_get:tpn.protocol.SearchResponse.results)
  return _internal_results(index);
}
inline ::tpn::protocol::Result* SearchResponse::_internal_add_results() {
  return results_.Add();
}
inline ::tpn::protocol::Result* SearchResponse::add_results() {
  ::tpn::protocol::Result* _add = _internal_add_results();
  // @@protoc_insertion_point(field_add:tpn.protocol.SearchResponse.results)
  return _add;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::tpn::protocol::Result >&
SearchResponse::results() const {
  // @@protoc_insertion_point(field_list:tpn.protocol.SearchResponse.results)
  return results_;
}

// -------------------------------------------------------------------

// Result

// string url = 1;
inline void Result::clear_url() {
  url_.ClearToEmpty();
}
inline const std::string& Result::url() const {
  // @@protoc_insertion_point(field_get:tpn.protocol.Result.url)
  return _internal_url();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Result::set_url(ArgT0&& arg0, ArgT... args) {
 
 url_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:tpn.protocol.Result.url)
}
inline std::string* Result::mutable_url() {
  std::string* _s = _internal_mutable_url();
  // @@protoc_insertion_point(field_mutable:tpn.protocol.Result.url)
  return _s;
}
inline const std::string& Result::_internal_url() const {
  return url_.Get();
}
inline void Result::_internal_set_url(const std::string& value) {
  
  url_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* Result::_internal_mutable_url() {
  
  return url_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* Result::release_url() {
  // @@protoc_insertion_point(field_release:tpn.protocol.Result.url)
  return url_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void Result::set_allocated_url(std::string* url) {
  if (url != nullptr) {
    
  } else {
    
  }
  url_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), url,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:tpn.protocol.Result.url)
}

// string title = 2;
inline void Result::clear_title() {
  title_.ClearToEmpty();
}
inline const std::string& Result::title() const {
  // @@protoc_insertion_point(field_get:tpn.protocol.Result.title)
  return _internal_title();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Result::set_title(ArgT0&& arg0, ArgT... args) {
 
 title_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:tpn.protocol.Result.title)
}
inline std::string* Result::mutable_title() {
  std::string* _s = _internal_mutable_title();
  // @@protoc_insertion_point(field_mutable:tpn.protocol.Result.title)
  return _s;
}
inline const std::string& Result::_internal_title() const {
  return title_.Get();
}
inline void Result::_internal_set_title(const std::string& value) {
  
  title_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* Result::_internal_mutable_title() {
  
  return title_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* Result::release_title() {
  // @@protoc_insertion_point(field_release:tpn.protocol.Result.title)
  return title_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void Result::set_allocated_title(std::string* title) {
  if (title != nullptr) {
    
  } else {
    
  }
  title_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), title,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:tpn.protocol.Result.title)
}

// repeated string snippets = 3;
inline int Result::_internal_snippets_size() const {
  return snippets_.size();
}
inline int Result::snippets_size() const {
  return _internal_snippets_size();
}
inline void Result::clear_snippets() {
  snippets_.Clear();
}
inline std::string* Result::add_snippets() {
  std::string* _s = _internal_add_snippets();
  // @@protoc_insertion_point(field_add_mutable:tpn.protocol.Result.snippets)
  return _s;
}
inline const std::string& Result::_internal_snippets(int index) const {
  return snippets_.Get(index);
}
inline const std::string& Result::snippets(int index) const {
  // @@protoc_insertion_point(field_get:tpn.protocol.Result.snippets)
  return _internal_snippets(index);
}
inline std::string* Result::mutable_snippets(int index) {
  // @@protoc_insertion_point(field_mutable:tpn.protocol.Result.snippets)
  return snippets_.Mutable(index);
}
inline void Result::set_snippets(int index, const std::string& value) {
  snippets_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set:tpn.protocol.Result.snippets)
}
inline void Result::set_snippets(int index, std::string&& value) {
  snippets_.Mutable(index)->assign(std::move(value));
  // @@protoc_insertion_point(field_set:tpn.protocol.Result.snippets)
}
inline void Result::set_snippets(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  snippets_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:tpn.protocol.Result.snippets)
}
inline void Result::set_snippets(int index, const char* value, size_t size) {
  snippets_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:tpn.protocol.Result.snippets)
}
inline std::string* Result::_internal_add_snippets() {
  return snippets_.Add();
}
inline void Result::add_snippets(const std::string& value) {
  snippets_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:tpn.protocol.Result.snippets)
}
inline void Result::add_snippets(std::string&& value) {
  snippets_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:tpn.protocol.Result.snippets)
}
inline void Result::add_snippets(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  snippets_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:tpn.protocol.Result.snippets)
}
inline void Result::add_snippets(const char* value, size_t size) {
  snippets_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:tpn.protocol.Result.snippets)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>&
Result::snippets() const {
  // @@protoc_insertion_point(field_list:tpn.protocol.Result.snippets)
  return snippets_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>*
Result::mutable_snippets() {
  // @@protoc_insertion_point(field_mutable_list:tpn.protocol.Result.snippets)
  return &snippets_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace protocol
}  // namespace tpn

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_protocol_2ftest_5fservice_2eproto
