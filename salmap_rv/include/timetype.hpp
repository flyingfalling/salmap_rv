#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>



namespace salmap_rv
{
enum _TIMETYPE { ABSTIME, RELTIME, ABSIDX, RELIDX };
struct TIMETYPE {
_TIMETYPE t; 
int64_t offset; 
 TIMETYPE();
 TIMETYPE( const int64_t& curr_time );
 TIMETYPE( const std::string& type, const std::string& off );
std::string print();
std::string get_timeval();
void set_timeval( const std::string& off );
const bool is_time() const;
const bool is_idx() const;
int64_t time_from_current( const int64_t& current ) const;
int64_t idx_from_current( int64_t& current );
void set_timetype( const std::string& type );
std::string get_timetype();
};
}
