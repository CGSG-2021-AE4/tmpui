#ifndef __ui_controls_empty_h_
#define __ui_controls_empty_h_

#include "../entity.h"

namespace ui
{
  namespace controls
  {
    struct empty_props
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {0};
      layout_props LayoutProps {};
      box_props BoxProps {};
    }; /* End of 'entity_props' struct */

    /* Empty class */
    class empty : public entity
    {
    public:
  
      /* Contsructor function */
      empty( const empty_props &NewProps, const std::vector<entity *> &NewChildren = {} ) :
        entity(NewProps, NewChildren)
      {
      } /* End of 'empty' function */

    }; /* End of 'empty' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_empty_h_