// (c) 2015-06-24 Jens Hauke <jens.hauke@4k2.de>
// Module for ILI9163c 128x128 color spi display

#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "auxmods.h"
#include "lrotable.h"

//#include "c_string.h"
#include "c_stdlib.h"


// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"

#include "ili9163.h"

// Lua: ili9163.begin( self )
static int ili_begin( lua_State *L )
{
    ili9163_begin();
    return 0;
}


const LUA_REG_TYPE ili9163_map[] =
{
    { LSTRKEY( "begin" ),  LFUNCVAL( ili_begin ) },
    { LSTRKEY( "hw" ),       LNUMVAL( 43110 ) },
    { LSTRKEY( "__index" ), LROVAL ( ili9163_map ) },
    { LNILKEY, LNILVAL }
};


LUALIB_API int luaopen_ili9163( lua_State *L )
{
    luaL_rometatable(L, AUXLIB_ILI9163, (void *)ili9163_map);  // create metatable
    return 0;

    /* luaL_register( L, AUXLIB_ILI9163, (void *)ili9163_map ); */

    /* // Set it as its own metatable */
    /* lua_pushvalue( L, -1 ); */
    /* lua_setmetatable( L, -2 ); */

    return 1;
}
