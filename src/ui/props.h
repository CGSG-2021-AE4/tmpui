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

  /* Different states styles */
  struct state_style
  {
    vec3
      DefColor = {0},    // Default state color
      HoverColor = {0},  // Hover state color
      ActiveColor = {0}; // Active state color
  }; /* End of 'state_style' struct */

  struct box_style
  {
    state_style
      Space,  // Space style
      Border; // Border style
  }; /* End of 'box_style' struct */

  /* Entity props structure */
  template<typename additional_props_type, typename style_props_type>
    struct entity_props
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {0};
      layout_props LayoutProps {};
      box_props BoxProps {};

      additional_props_type Props {};
      style_props_type Style {};
    }; /* End of 'entity_props' struct */
  
} /* end of 'ui' namespace */

#endif // __ui_props_h_