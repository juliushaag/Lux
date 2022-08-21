#pragma once 

#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(default: 4201)

#include <memory>
#include <array>
#include <unordered_map>
#include <bitset>
#include <set>

namespace Lux
{

#define BIT(x) (1 << x)

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, int count>
using Array = std::array<T, count>;

template<typename T, typename N>
using Container = std::unordered_map<T, N>;

template<typename T>
using List = std::vector<T>;

template<int T>
using Bitset = std::bitset<T>;

template<typename T>
using Set = std::set<T>;

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint32_t;
using u8 = uint8_t;

using byte = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int32_t;
using i8 = int8_t;

using f32 = float_t;

using mat4 = glm::mat4;

using v2 = glm::vec2;

using v3 = glm::vec3;

using v4 = glm::vec4;

enum class DataType : u32
{
    INVALID,
    FLOAT,
    INT,
    BYTE,
    CHAR,
    BOOL,
    V2,
    V3,
    V4,
    MAT2,
    MAT3,
    MAT4,
    SAMPLER1D,
    SAMPLER2D,
    SAMPLER3D,
};


template<typename T, typename ... Args>
constexpr Scope<T> create_scope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}


template<typename T, typename ... Args>
constexpr Ref<T> create_scope(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}


template<typename T, typename N>
inline bool map_contains(std::unordered_map<T, N>& map, T& key)
{
    return map.find(key) != map.end();
}

}