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

  /* Overflow type enum struct */
  enum struct overflow_type
  {
    eHidden, // User can't scroll content
    eScroll, // User can scroll content
  }; /* End of 'overflow_type' enum struct */

  /* Box props */
  struct box_props
  {
    INT
      MarginW = 0,  // Margin width
      BorderW = 0,  // Border width
      PaddingW = 0; // Padding width
  }; /* End of 'box_props' struct */

  /* Flex basis type enum struct */
  enum struct flex_basis_type
  {
    eFixed,      // Uses initial size
    eMaxContent, // Uses max content
  }; /* End of 'flex_basis_type' enum struct */

  /* Flex props struct */
  struct flex_props
  {
    flex_basis_type Basis = flex_basis_type::eMaxContent;
    FLT
      Grow = 0,
      Shrink = 0;
  }; /* End of 'flex_props' struct */

  /* Entity props structure */
  template<typename additional_props_type, typename style_props_type>
    struct entity_props
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2
        Size {0},
        MinSize {0},
        MaxSize {0};
      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex {};
      box_props BoxProps {};

      additional_props_type Props {};
      style_props_type Style {};
    }; /* End of 'entity_props' struct */

  /* Different states styles */
  struct state_style
  {
    vec3
      DefColor = {0},    // Default state color
      HoverColor = {0},  // Hover state color
      ActiveColor = {0}; // Active state color
  }; /* End of 'state_style' struct */

  /* Box style struct */
  struct box_style
  {
    state_style
      Space,  // Space style
      Border; // Border style
  }; /* End of 'box_style' struct */

} /* end of 'ui' namespace */

#endif // __ui_props_h_