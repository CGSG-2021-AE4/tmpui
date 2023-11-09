#include <string_view>

#ifndef __ui_controls_text_h_
#define __ui_controls_text_h_

#include "../entity.h"
#include "../frame_render.h"

namespace ui
{
  namespace controls
  {
    /* Text style structure */
    struct text_style
    {
      vec3 Color {0};
      DWORD LayoutFlags {0};
      ivec2 Padding {2, 0};
    }; /* End of 'text_style' struct */

    /* Text props structure */
    struct text_props
    {
      BOOL IsSingleLine {false};
      std::string Str {""};
    }; /* End of 'div_props' struct */

    class text;

  } /* end of 'controls' namespace */

  template<>
    struct entity_props<controls::text>
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {10};
      min_size_type MinSize {min_size_ref::eMinContent};
      isize2 MaxSize {10000};
      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex {};
      box_props BoxProps {};

      controls::text_props Props {};
      controls::text_style Style {};
    }; /* End of 'entity_props' struct */

  namespace controls
  {
    /* Text class */
    class text : public entity
    {
      text_style Style;
      text_props Props;
      std::string Str;
      std::vector<std::string_view> Lines; // Isn't used if text is a single line

    public:
  
      /* Contsructor function */
      text( const entity_props<text> &NewProps ) :
        entity(NewProps),
        Props(NewProps.Props),
        Style(NewProps.Style),
        Str(Props.Str)
      {
        IsBackgroundTransparent = true;
      } /* End of 'text' function */

      isize2 GetMaxContent( VOID ) override
      {
        return {(INT)Str.size() * render_2d::FontW + Style.Padding.X, render_2d::FontH};
      } /* End of 'UpdateMinMaxContent' function */

      VOID UpdateLines( VOID )
      {
        INT CharsPerLine = (INT)((Size.W - Style.Padding.X * 2) / render_2d::FontW);

        if (Props.IsSingleLine || CharsPerLine <= 0)
        {
          Lines = {Str};
          return;
        }

        Lines.clear();

        INT PrevWordStart = -1, PrevWordEnd = -1;

        for (INT LineStartOffset = 0; LineStartOffset < (INT)Str.size();)
        {
          INT Offset = LineStartOffset;
          BOOL IsPrevCSpace = 1;

          while (Offset < (INT)Str.size() && Offset - LineStartOffset < CharsPerLine && Str[Offset] != '\n')
          {
            BOOL IsSpace = !isalpha(Str[Offset]);
            
            if (IsPrevCSpace && !IsSpace)
              PrevWordStart = Offset;
            else if (!IsPrevCSpace && IsSpace)
              PrevWordEnd = Offset;

            IsPrevCSpace = IsSpace;
            Offset++;
          }

          if (Offset < (INT)Str.size())
          {
            if (Str[Offset] == '\n')
            {
              Lines.push_back(std::string_view(Str).substr(LineStartOffset, Offset - LineStartOffset));
              LineStartOffset = Offset + 1;
            }
            else if (PrevWordEnd > PrevWordStart) // We are out of a word
            {
              Lines.push_back(std::string_view(Str).substr(LineStartOffset, PrevWordEnd - LineStartOffset));
              LineStartOffset = Offset;
            }
            else if (PrevWordEnd < PrevWordStart) // We are in a word
            {
              Lines.push_back(std::string_view(Str).substr(LineStartOffset, PrevWordEnd - LineStartOffset));

              if (PrevWordStart <= LineStartOffset)
                LineStartOffset = Offset;
              else
                LineStartOffset = PrevWordStart;
            }
          }
          else
          {
            Lines.push_back(std::string_view(Str).substr(LineStartOffset, Offset - LineStartOffset));
            LineStartOffset = Offset;
          }
        }
      } /* End of 'UpdateLines' function */

      VOID OnResize( VOID ) override
      {
        UpdateLines();
      } /* End of 'OnResize' function */
      
      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(vec3(0.8)), ToRGB(vec3(0.6)), BoxProps.BorderW, SelfDrawMask);

        for (INT Line = 0; Line < (INT)Lines.size(); Line++)
          Canvas->Render2d.PutStr(Lines[Line], GlobalContentPos + ivec2(0, Line * render_2d::FontH), InnerSize, Style.Padding, Style.LayoutFlags, 0, SelfDrawMask);
      } /* End of 'OnDraw' function */

      /* Set text function */
      VOID SetStr( const std::string &NewStr )
      {
        Str = NewStr;
        OnResize();
        //UpdateMinMaxContentRec();
        OnUpdateContent();
        //Redraw();
      } /* End of 'SetStr' function */

    }; /* End of 'text' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_text_h_