#ifndef __ui_controls_div_h_
#define __ui_controls_div_h_

#include "../entity.h"

namespace ui
{
  namespace controls
  {
    /* Div style structure */
    struct div_style
    {
      vec3
        SpaceColor,
        BorderColor;
    }; /* End of 'div_style' struct */

    class div;
  } /* end of 'constrols' namespace */

  template<>
    struct entity_props<controls::div>
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {0};
      min_size_type MinSize {min_size_ref::eNone};
      isize2 MaxSize {10000};

      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex {};
      box_props BoxProps {};

      controls::div_style Style {};
    };

  namespace controls
  {
    /* Div class */
    class div : public entity
    {
      div_style Style;

    public:
  
      /* Contsructor function */
      div( const entity_props<div> &NewProps ) :
        entity(NewProps),
        Style(NewProps.Style)
      {
      } /* End of 'div' function */

      /* Get in content override function */
      isize2 GetMinContent( VOID ) override
      {
        isize2 NewMinContent = 0;
        switch (LayoutType)
        {
        case layout_type::eBlock:
          break;
        case layout_type::eFlexRow:
          for (entity *Child : Children)
          {
            NewMinContent.W += Child->MinContent.W;
            NewMinContent.H = std::max(NewMinContent.H, Child->MinContent.H);
          }
          break;
        case layout_type::eFlexColumn:
          for (entity *Child : Children)
          {
            isize2 ChildMaxContent = Child->GetPreferedSize();

            NewMinContent.W = std::max(NewMinContent.W, ChildMaxContent.W);
            NewMinContent.H += ChildMaxContent.H;
          }
          break;
        }
        return NewMinContent;
      } /* End of 'GetMinContent' function */

      /* Get max content override function */
      isize2 GetMaxContent( VOID ) override
      {
        isize2 NewMaxContent = 0;
        switch (LayoutType)
        {
        case layout_type::eBlock:
          for (entity *Child : Children)
          {
            isize2 ChildMaxContent = Child->GetPreferedSize();

            NewMaxContent = {std::max(NewMaxContent.W, Child->LocalPos.X + ChildMaxContent.W),
                             std::max(NewMaxContent.H, Child->LocalPos.Y + ChildMaxContent.H)};
          }
          break;
        case layout_type::eFlexRow:
          for (entity *Child : Children)
          {
            isize2 ChildMaxContent = Child->GetPreferedSize();

            NewMaxContent.W += ChildMaxContent.W;
            NewMaxContent.H = std::max(NewMaxContent.H, ChildMaxContent.H);
          }
          break;
        case layout_type::eFlexColumn:
          for (entity *Child : Children)
          {
            isize2 ChildMaxContent = Child->GetPreferedSize();

            NewMaxContent.W = std::max(NewMaxContent.W, ChildMaxContent.W);
            NewMaxContent.H += ChildMaxContent.H;
          }
          break;
        }
        return NewMaxContent;
      } /* End of 'GetMaxContent' function */
      
      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(Style.BorderColor), ToRGB(Style.SpaceColor), BoxProps.BorderW, SelfDrawMask);

        // Canvas->Render2d.PutBar(GlobalPos, GetPreferedSize(), (DWORD)RGB(0, 255, 0), SelfDrawMask);
        // Canvas->Render2d.PutBar(GlobalPos, GetMinSize(), (DWORD)RGB(255, 0, 0), SelfDrawMask);
        // Canvas->Render2d.PutBar(GlobalPos, GetMaxSize(), (DWORD)RGB(0, 0, 255), SelfDrawMask);
      } /* End of 'OnDraw' function */

    }; /* End of 'div' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_div_h_