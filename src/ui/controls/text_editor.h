#include <string_view>

#ifndef __ui_controls_text_editor_h_
#define __ui_controls_text_editor_h_

#include "../entity.h"
#include "../frame_render.h"

namespace ui
{
  namespace controls
  {
    /* Text style structure */
    struct text_editor_style
    {
      vec3 Color {0};
    }; /* End of 'text_editor_style' struct */

    /* Text props structure */
    struct text_editor_props
    {
      std::string Str {""};
      INT WidthChars;
    }; /* End of 'div_editor_props' struct */

    class line_editor;

  } /* end of 'controls' namespace */

  template<>
    struct entity_props<controls::line_editor>
    {
      std::string Id {""};
      ivec2 Pos {0};
      isize2 Size {10};
      size_type MinSize {size_ref::eMinContent};
      isize2 MaxSize {10000};
      layout_type LayoutType = layout_type::eBlock;
      overflow_type Overflow = overflow_type::eHidden;
      flex_props Flex {};
      box_props BoxProps {};

      controls::text_editor_props Props {};
      controls::text_editor_style Style {};
    }; /* End of 'entity_props' struct */

  namespace controls
  {
    /* Text editor class */
    class line_editor : public entity
    {
      text_editor_style Style;
      text_editor_props Props;
      std::string Str;
      INT
        CursorPos = 0,
        SelectCursorPos = 0;
      BOOL IsCursorSet = false;
      INT Offset = 0;
      INT CharsPerLine = 0;

    public:
  
      /* Contsructor function */
      line_editor( const entity_props<line_editor> &NewProps ) :
        entity(NewProps),
        Props(NewProps.Props),
        Style(NewProps.Style)
      {
        IsBackgroundTransparent = true;
        Str = Props.Str;
      } /* End of 'text' function */

      isize2 GetMaxContent( VOID ) override
      {
        return {render_2d::FontW * Props.WidthChars, render_2d::FontH};
      } /* End of 'UpdateMinMaxContent' function */
      
      isize2 GetMinContent( VOID ) override
      {
        return {0, render_2d::FontH};
      } /* End of 'UpdateMinMaxContent' function */

      const std::string & GetStr( VOID ) const
      {
        return Str;
      } /* End of 'GetStr' function */

      /* On draw event function */
      VOID OnDraw( VOID ) override
      {
        if (IsCursorSet)
          Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(vec3(0.9)), BoxProps.BorderW, SelfDrawMask);
        else if (State == entity_state::eHovered)
          Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(vec3(0.7)), BoxProps.BorderW, SelfDrawMask);
        else
          Canvas->Render2d.PutBar(GlobalPos, Size, ToRGB(vec3(0.4)), BoxProps.BorderW, SelfDrawMask);

        Canvas->Render2d.PutStr(Str, GlobalContentPos - ivec2(Offset * render_2d::FontW, 0), InnerSize, 0, 0, 0, ContentDrawMask);
        if (IsCursorSet)
          Canvas->Render2d.PutBar(GlobalContentPos + ivec2((std::min(CursorPos, SelectCursorPos) - Offset) * render_2d::FontW, 0), isize2(std::max(BoxProps.BorderW, std::abs(CursorPos - SelectCursorPos) * render_2d::FontW), render_2d::FontH), ToRGB(vec3(1, 0.5, 0.5)), BoxProps.BorderW, ContentDrawMask);
      } /* End of 'OnDraw' function */

      /* Set text function */
      VOID SetStr( const std::string &NewStr )
      {
        Str = NewStr;
        //OnUpdateContent();
        Redraw();
      } /* End of 'SetStr' function */

      VOID UpdateOffset( VOID )
      {
        if (SelectCursorPos - Offset >= CharsPerLine)
          Offset = SelectCursorPos - CharsPerLine + 1;
        else if (SelectCursorPos - Offset < 0)
          Offset = SelectCursorPos;
      } /* End of 'UpdateOffset' function */

      inline VOID SetCursorPos( INT NewCursorPos )
      {
        SelectCursorPos = CursorPos = std::clamp(NewCursorPos, 0, (INT)Str.size());
        
        UpdateOffset();
      } /* End of 'SetCursorPos' function */

      inline VOID SetSelectCursorPos( INT NewCursorPos ) // For later
      {
        SelectCursorPos = std::clamp(NewCursorPos, 0, (INT)Str.size());
        
        UpdateOffset();
      } /* End of 'SetSelectCursorPos' function */

      inline VOID MoveCursor( INT NewPos ) // Temp shit
      {
        if (GetKeyState(VK_SHIFT) & 0x8000)
          SetSelectCursorPos(NewPos);
        else
          SetCursorPos(NewPos);
      } /* End of 'MoveCursor' function */

      VOID Paste( const std::string &InStr )
      {
        Str = Str.substr(0, CursorPos) + InStr + Str.substr(CursorPos, (INT)Str.size() - CursorPos);

        SetCursorPos(CursorPos + (INT)InStr.size());
      } /* End of 'Paste' function */

      std::string Cut( VOID )
      {
        std::string CutStr = Str.erase(std::min(CursorPos, SelectCursorPos), std::abs(CursorPos - SelectCursorPos));

        SetCursorPos(std::min(CursorPos, SelectCursorPos));
        return CutStr;
      } /* End of 'Cut' function */

      /* Event functions */

      BOOL OnInput( UINT Key ) override // Temp response function
      {
        switch (Key)
        {
        case VK_RIGHT:
          if (GetKeyState(VK_CONTROL) & 0x8000)
          {
            INT cp = SelectCursorPos + 1;

            while (cp < (INT)Str.size() && isalpha(Str[cp]))
              cp++;
            MoveCursor(cp);
          }
          else
            MoveCursor(SelectCursorPos + 1);
          break;
        case VK_LEFT:
          if (GetKeyState(VK_CONTROL) & 0x8000)
          {
            INT cp = SelectCursorPos - 1;

            while (cp >= 0 && isalpha(Str[cp] % 256))
              cp--;
            MoveCursor(cp);
          }
          else
            MoveCursor(SelectCursorPos - 1);
          break;
        case VK_BACK:
          if (CursorPos != SelectCursorPos)
            Cut();
          else if (CursorPos > 0)
          {
            Str.erase(CursorPos - 1, 1);
            if (Offset > 0)
              Offset--;
          }

          SetCursorPos(CursorPos - 1);
          break;
        case VK_DELETE:
          if (CursorPos != SelectCursorPos)
            Cut();
          else if ((INT)Str.size() - CursorPos > 0)
            Str.erase(CursorPos, 1);
          break;
        case VK_CONTROL: // Some functional keys - need to skip
        case VK_SHIFT:
        case VK_RSHIFT:
        case VK_RCONTROL:
        case VK_CAPITAL:
        case VK_TAB:
          break;
        case VK_ESCAPE:
          IsCursorSet = FALSE;
          break;
        case VK_HOME:
          MoveCursor(0);
          break;
        case VK_END:
          MoveCursor((INT)Str.size());
          break;
        default:
          if (isalpha(Key) && !(GetKeyState(VK_CAPITAL) & 0x0001 || GetKeyState(VK_SHIFT) & 0x8000))
            Key = tolower(Key);

          Str = Str.substr(0, CursorPos) + (CHAR)Key + Str.substr(CursorPos, (INT)Str.size() - CursorPos);
          SetCursorPos(CursorPos + 1);
          break;
        }
        return true;
      } /* End of 'OnInput' function */

      BOOL OnHover( const ivec2 &LocalMousePos ) override
      {
        return true;
      } /* End of 'OnHover' function */

      BOOL OnUnhover( const ivec2 &LocalMousePos ) override
      {
        return true;
      } /* End of 'OnUnhover' function */

      BOOL OnUnfocus( VOID ) override
      {
        IsCursorSet = false;
        return true;
      } /* End of 'OnUnfocus' function */

      BOOL OnMouseDown( const ivec2 &LocalMousePos ) override
      {
        if (IsCursorSet)
          MoveCursor((FLT)(LocalMousePos - BoxProps.BorderW - BoxProps.PaddingW).X / render_2d::FontW + Offset);
        else
          SetCursorPos((FLT)(LocalMousePos - BoxProps.BorderW - BoxProps.PaddingW).X / render_2d::FontW + Offset);

        IsCursorSet = true;
        
        return true;
      } /* End of 'OnMouseDown' function */

      virtual BOOL OnDrag( const ivec3 &Delta, const ivec2 &LocalMousePos )
      {
        SetSelectCursorPos((FLT)(LocalMousePos - BoxProps.BorderW - BoxProps.PaddingW).X / render_2d::FontW + Offset);

        return true;
      } /* End of 'OnDrag' function */

      VOID OnResize( VOID ) override
      {
        CharsPerLine = InnerSize.W / render_2d::FontW;

        UpdateOffset();
      } /* End of 'OnResize' function */

    }; /* End of 'line_editor' class */

  } /* end of 'controls' namespace */
} /* end of 'ui' namespace */

#endif // __ui_controls_text_h_