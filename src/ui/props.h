// File with different props and style info structures

#include "ui_def.h"

#ifndef __ui_props_h_
#define __ui_props_h_

namespace ui
{
  /* Layout type enum struct */
  enum struct layout_type
  {
    eBlock,
    eFlexRow,
    eFlexColumn,
  }; /* End of 'layout_type' enum struct */

  /* Layout props struct */
  struct layout_props
  {
    layout_type Type = layout_type::eBlock; // Type of entity layout
    FLT Flex         = 0;                   // Flex coef
    isize2 MinSize   = {0, 0};              // Min size
    isize2 MaxSize   = {-1, -1};            // Max size
    BOOL IsScrollable = 0;                  // Can the entity be scrolled
  }; /* End of 'layout_props' struct */

  /* Box props */
  struct box_props
  {
    INT
      MarginW = 0,  // Margin width
      BorderW = 0,  // Border width
      PaddingW = 0; // Padding width
  }; /* End of 'box_props' struct */

} /* end of 'ui' namespace */

#endif // __ui_props_h_