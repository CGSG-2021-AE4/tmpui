#include <map>
#include <optional>
#include <functional>

#ifndef __ui_components_radio_button_h_
#define __ui_components_radio_button_h_

#include "../entity.h"

// Controls
#include "../controls/div.h"
//#include "../controls/text.h"
#include "../controls/button.h"
// #include "../controls/text_editor.h"

namespace ui
{
  namespace components
  {
    /* Options props */
    struct radio_button_option_props
    {
      std::string Name {};
      std::optional<INT> Value;
      std::optional<std::function<VOID( VOID )>> OnSelectCallBack;
    }; /* End of 'option_props' struct */

    class radio_button;
  } /* end of 'controls' namespace */

  template<>
    struct entity_props<components::radio_button>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
      dir_type Dir {dir_type::eHorizontal};
      box_props BoxProps { .MarginW = 4, };

      std::vector<components::radio_button_option_props> Options;

      std::function<VOID( INT Value )> OnSelectCallBack {[]( INT Value ){}};
    }; /* End of 'entity_props' struct */

  namespace components
  {
    /* Tabs component class */
    class radio_button : public controls::div
    {
      std::function<VOID( INT Value )> OnSelectCallBack;

      controls::button *CurOption {nullptr};

    public:

      /* Radio button class construct function */
      radio_button( const entity_props<radio_button> &NewProps ) :
        controls::div({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .LayoutType = (NewProps.Dir == dir_type::eVertical) ? ::ui::layout_type::eFlexColumn : ::ui::layout_type::eFlexRow,
          .Flex = NewProps.Flex,
          .BoxProps = NewProps.BoxProps,
        }),
        OnSelectCallBack(NewProps.OnSelectCallBack)
      {
        IsBackgroundTransparent = true;
        AddOptions(NewProps.Options);
      } /* End of 'radio_button' function */

      /* Add options function */
      VOID AddOptions( const std::vector<radio_button_option_props> Options )
      {
        std::vector<entity *> NewButtons;

        for (INT i = 0, Offset = (INT)Children.size(); i < (INT)Options.size(); i++)
        {
          auto Option = Options[i];

          NewButtons.push_back(Create<controls::button>({
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Value = 0,
            .Props = {
              .IsPress = true,
              .Str = Options[i].Name,
              .OnChangeCallBack = [Option, i, this]( controls::button *Button ) {
                if (CurOption == Button)
                {
                  // We click the same button second time
                  Button->SetValueUnsafe(true);
                  return;
                }

                if (CurOption != nullptr)
                  CurOption->SetValue(false);
                CurOption = Button;

                if (Option.OnSelectCallBack.has_value())
                {
                  // It has on select call back
                  Option.OnSelectCallBack.value()();
                }
                else
                {
                  // It doesn't have
                  if (Option.Value.has_value())
                    OnSelectCallBack(Option.Value.value()); // If it has special value we call with special value
                  else
                    OnSelectCallBack(i); // Otherwise with index
                }

              } // End of 'OnClickCallBack' labda
            },
          }));
        }
        AddChildren(NewButtons);
      } /* End of 'AddOptions' function */

      /* Set current option function */
      VOID SetCurOption( INT Index )
      {
        if (Index < (INT)Children.size())
        {
          controls::button *NewOption = reinterpret_cast<controls::button *>(Children[Index]);

          if (NewOption == CurOption)
            return;
          if (CurOption != nullptr)
            CurOption->SetValue(false);
          NewOption->SetValue(true);
          CurOption = NewOption;
        }
      } /* End of 'SetCurOption' function */

    }; /* End of 'radio_button' class */

  } /* end of 'components' namespace */
} /* end of 'ui' namespace */

#endif // __ui_components_tabs_h_