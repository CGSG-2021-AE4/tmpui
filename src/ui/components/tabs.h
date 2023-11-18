#include <map>

#ifndef __ui_components_tabs_h_
#define __ui_components_tabs_h_

#include "../entity.h"

// Controls
#include "../controls/div.h"
#include "../controls/text.h"
#include "../controls/button.h"
// #include "../controls/text_editor.h"

namespace ui
{
  namespace components
  {
    class tabs;
  } /* end of 'controls' namespace */

  template<>
    struct entity_props<components::tabs>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
      dir_type TabsDir {dir_type::eHorizontal};
      std::vector<std::pair<std::string, entity *>> Tabs {};
    }; /* End of 'entity_props' struct */

  namespace components
  {
    /* Tabs component class */
    class tabs : public controls::div
    {
      controls::div
        *ButtonsDiv,
        *ContentDiv;

      entity *CurTabE {nullptr};

      std::map<entity *, entity *> TabsToButtons; // Map from tab entity to it's button entity

    public:

      /* Tabs class construct function */
      tabs( const entity_props<tabs> &NewProps ) :
        controls::div({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .LayoutType = (NewProps.TabsDir == dir_type::eHorizontal) ? ::ui::layout_type::eFlexColumn : ::ui::layout_type::eFlexRow,
          .Flex = NewProps.Flex,
          .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.3, 0.3, 0.75} },
        })
      {
        // Create base divs
        ButtonsDiv = Create<controls::div>({
          .Id = "Tabs div",
          .LayoutType = (NewProps.TabsDir == dir_type::eHorizontal) ? ::ui::layout_type::eFlexRow : ::ui::layout_type::eFlexColumn,
          .Overflow = ::ui::overflow_type::eScroll,
          .ScrollDir = (NewProps.TabsDir == dir_type::eHorizontal) ? ::ui::dir_type::eHorizontal : ::ui::dir_type::eVertical,
          // .Flex = { .Basis = ::ui::flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
          .BoxProps = { .MarginW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
        });
        ContentDiv = Create<controls::div>({
          .Id = "Content div",
          .LayoutType = ::ui::layout_type::eFlexColumn,
          .Flex = { .Grow = 1 },
          .BoxProps = { .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
        });

        // Add tabs
        AddTabs(NewProps.Tabs);
        if (ContentDiv->Children.size() > 0)
          SetCurTab(ContentDiv->Children[0]); // Some shit - first tab
        AddChildren({ ButtonsDiv, ContentDiv });
      } /* End of 'text_test' function */

      /* Add new tabs function */
      VOID AddTabs( const std::vector<std::pair<std::string, entity *>> &NewTabs )
      {
        std::vector<entity *>
          TabsEs {},
          ButtonsEs {};

        for (auto Tab : NewTabs)
        {
          entity *ButtonE = Create<controls::button>({
            .Id = "Tab" + Tab.first + " button",
            //.Flex = { .Grow = 1 },
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Value = 0,
            .Props = { .IsPress = true, .Str = Tab.first, .OnClickCallBack = [this, Tab]( controls::button *Button ){
                if (TabsToButtons.find(CurTabE) != TabsToButtons.end())
                  reinterpret_cast<::ui::controls::button *>(TabsToButtons[CurTabE])->SetValue(false);
                SetCurTabUnsafe(Tab.second);
                ContentDiv->OnUpdateContent();
                ContentDiv->Redraw();
              }
            },
          });

          Tab.second->SetVisibility(false);
          TabsToButtons.emplace(Tab.second, ButtonE);
          ButtonsEs.push_back(ButtonE);
          TabsEs.push_back(Tab.second);
        }
        ButtonsDiv->AddChildren(ButtonsEs);
        ContentDiv->AddChildren(TabsEs);
      } /* End of 'AddTabs' function */

    private:

      /* Unsafe set current tab function */
      VOID SetCurTabUnsafe( entity *E )
      {
        if (CurTabE != nullptr)
          CurTabE->SetVisibility(false);
        E->SetVisibility(true);
        CurTabE = E;
      } /* End of 'SetCurTabUnsafe' function */

    public:

      /* Set current tab function */
      VOID SetCurTab( entity *E )
      {
        if (CurTabE != nullptr)
        {
          CurTabE->SetVisibility(false);
          if (TabsToButtons.find(CurTabE) != TabsToButtons.end())
            reinterpret_cast<::ui::controls::button *>(TabsToButtons[CurTabE])->SetValue(false);
        }
        E->SetVisibility(true);
        if (TabsToButtons.find(E) != TabsToButtons.end())
            reinterpret_cast<::ui::controls::button *>(TabsToButtons[E])->SetValue(true);
        CurTabE = E;
        ContentDiv->OnUpdateContent();
        ContentDiv->Redraw();
      } /* End of 'SetCurTabUnsafe' function */

    }; /* End of 'text_test' class */

  } /* end of 'components' namespace */
} /* end of 'ui' namespace */

#endif // __ui_components_tabs_h_