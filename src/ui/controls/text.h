#include <string_view>

#ifndef __ui_controls_text_h_
#define __ui_controls_text_h_

#include "../entity.h"

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
      text_style Style {};
      std::string Str {""};
    }; /* End of 'div_props' struct */

    /* Text class */
    class text : public entity
    {
      text_style Style;
      text_props Props;
      std::string Str;
      std::vector<std::string_view> Lines; // Isn't used if text is a single line

    public:
  
      /* Contsructor function */
      text( const entity_props<text_props, text_style> &NewProps ) :
        entity(NewProps),
        Props(NewProps.Props),
        Style(NewProps.Style),
        Str(Props.Str)
      {
        IsBackgroundTransparent = true;
      } /* End of 'text' function */

      VOID UpdateLines( VOID )
      {
        INT CharsPerLine = (INT)((Size.W - Style.Padding.X * 2) / render_2d::FontW);

        if (Props.IsSingleLine || CharsPerLine == 0)
        {
          Lines = {Str};
          return;
        }

        Lines.clear();

        INT PrevWordStart = 0, PrevWordEnd = 0;

        for (INT LineStartOffset = 0; LineStartOffset < (INT)Str.size();)
        {
          INT Offset = LineStartOffset;
          BOOL IsPrevCSpace = 0;

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

      virtual VOID OnResize( VOID )
      {
        UpdateLines();
        LayoutProps.MinSize.H = render_2d::FontH * (INT)Lines.size();
      } /* End of 'OnResize' function */
      
      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        for (INT Line = 0; Line < (INT)Lines.size(); Line++)
          Canvas->Render2d.PutStr(Lines[Line], GlobalPos + ivec2(0, Line * render_2d::FontH), Size, Style.Padding, Style.LayoutFlags, 0, SelfDrawMask);
      } /* End of 'OnDraw' function */

      /* Set text function */
      VOID SetStr( const std::string &NewStr )
      {
        Str = NewStr;
        OnResize();
        if (Parent != nullptr)
          Parent->UpdateChildrenLayout();
        Redraw();
      } /* End of 'SetStr' function */

    }; /* End of 'text' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_text_h_