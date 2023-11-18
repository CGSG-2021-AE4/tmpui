#ifndef __ui_components_text_test_h_
#define __ui_components_text_test_h_

#include "../entity.h"

// Controls
#include "../controls/div.h"
#include "../controls/text.h"
#include "../controls/button.h"
#include "../controls/text_editor.h"

namespace ui
{
  namespace components
  {
    class text_test;
  } /* end of 'controls' namespace */

  template<>
    struct entity_props<components::text_test>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
    }; /* End of 'entity_props' struct */

  namespace components
  {
    /* Text test component class */
    class text_test : public controls::div
    {
      controls::text *WholeText = nullptr;
      controls::line_editor *InputLine = nullptr;
    public:

      /* Text test class construct function */
      text_test( const entity_props<text_test> &NewProps ) :
        controls::div({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .Size = {300},
          .MaxSize = {1000},
          .LayoutType = ::ui::layout_type::eFlexColumn,
          .Flex = NewProps.Flex,
          .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.3, 0.3, 0.75} },
        })
      {
        AddChildren({
          Create<controls::text>({
            .Id = "Text test main bar",
            //.MaxSize = {100},
            //.Flex = { .Grow = 1 },
            .MaxSize = {5000},
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Props = { .Str = "Here you can type some text:"},
            .Style = { .Color = ::ui::vec3(1) }
          }),
          Create<controls::div>({
            .Id = "Header",
            .Size = {300},
            .MaxSize = {5000},
            .LayoutType = ::ui::layout_type::eFlexRow,
            //.Flex = { .Basis = ::ui::flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
          }, {
            InputLine = Create<controls::line_editor>({
              .Id = "Line editor",
              //.MinSize = isize2(100, 0),
              .MaxSize = {10000},
              .Flex = { .Grow = 1 },
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .Str = "Some text", .WidthChars = 10 },
              .Style = { .Color = ::ui::vec3(1) }
            }),
            Create<controls::button>({
              .Id = "Text test push button",
              //.Flex = { .Grow = 1 },
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .IsPress = true, .Str = "Push", .OnChangeCallBack = [&]( controls::button *Button ){
                //::ui::Log(std::format("New value: {}", Button->GetValue() ));
                if (!InputLine->GetStr().empty())
                {
                  WholeText->SetStr(WholeText->Str.WholeStr + "\n" + InputLine->GetStr());
                  InputLine->SetStr("");
                }
              } },
            }),
          }),
          Create<controls::div>({
            .Id = "Text test out div",
            //.Size = {300},
            .MaxSize = {5000},
            .LayoutType = ::ui::layout_type::eFlexColumn,
            .Flex = { .Grow = 1 },
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
          }, {
            WholeText = Create<controls::text>({
              .Id = "Text test out box",
              .Flex = { .Grow = 1, .Shrink = 1 },
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .Str = "I pounder of something great,\nmy lungs will fill and then deflate.\nThey fill with fire, exhale desire,\nI know it's dire my time today."},
              .Style = { .Color = ::ui::vec3(1) }
            }),
          }),
        });
      } /* End of 'text_test' function */
    }; /* End of 'text_test' class */

  } /* end of 'components' namespace */
} /* end of 'ui' namespace */

#endif // __ui_components_text_test_h_