#include <map>

#ifndef __ui_components_tabs_h_
#define __ui_components_tabs_h_

#include "../entity.h"

// Controls
#include "../controls/div.h"
#include "../controls/text.h"
#include "../controls/button.h"

// Components
#include "../components/radio_button.h"
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
      controls::div *ContentDiv;
      components::radio_button *SwitchButtonsE;

      entity *CurTabE {nullptr};

      std::map<entity *, INT> TabsToIndexes; // Map from tab entity to it's option indexes

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
        SwitchButtonsE = Create<components::radio_button>({
          .Dir = NewProps.TabsDir,
          .BoxProps = {},
        });
        ContentDiv = Create<controls::div>({
          .LayoutType = ::ui::layout_type::eFlexColumn,
          .Flex = { .Grow = 1, .Shrink = 1 },
          .BoxProps = { .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35} },
        });

        // Add tabs
        AddTabs(NewProps.Tabs);
        AddChildren({ SwitchButtonsE, ContentDiv });
        if (ContentDiv->Children.size() > 0)
          SetCurTab(ContentDiv->Children[0]); // Some shit - first tab
      } /* End of 'text_test' function */

      /* Add new tabs function */
      VOID AddTabs( const std::vector<std::pair<std::string, entity *>> &NewTabs )
      {
        std::vector<entity *> TabsEs {};
        std::vector<components::radio_button_option_props> ButtonsProps;

        for (INT i = 0, Offset = (INT)TabsToIndexes.size(); i < NewTabs.size(); i++)
        {
          NewTabs[i].second->SetVisibility(false);


          ButtonsProps.push_back({
            .Name = NewTabs[i].first,
            .OnSelectCallBack = [=]( VOID ){
              SetCurTabUnsafe(NewTabs[i].second);
              ContentDiv->OnUpdateContent();
              ContentDiv->Redraw();
            }
          });
          TabsEs.push_back(NewTabs[i].second);
          TabsToIndexes.emplace(NewTabs[i].second, Offset + i);
        }
        SwitchButtonsE->AddOptions(ButtonsProps);
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
        if (TabsToIndexes.find(E) != TabsToIndexes.end())
          SwitchButtonsE->SetCurOption(TabsToIndexes[E]);
        SetCurTabUnsafe(E);
        ContentDiv->OnUpdateContent();
        ContentDiv->Redraw();
      } /* End of 'SetCurTabUnsafe' function */

    }; /* End of 'text_test' class */

  } /* end of 'components' namespace */
} /* end of 'ui' namespace */

#endif // __ui_components_tabs_h_