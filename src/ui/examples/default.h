#include <map>
#include "../../socket/sl.h"


#ifndef __ui_examples_default_h_
#define __ui_examples_default_h_

#include "../entity.h"

// Controls
#include "../controls/div.h"
#include "../controls/text.h"
#include "../controls/button.h"
#include "../controls/slider.h"
#include "../controls/range.h"
#include "../controls/text_editor.h"

// Components
#include "../ui/components/text_test.h"
#include "../ui/components/tabs.h"

#include "../frame_render.h"

namespace ui
{
  namespace examples
  {
    class def;
  } /* end of 'examples' namespace */

  template<>
    struct entity_props<examples::def>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
    }; /* End of 'entity_props' struct */

  namespace examples
  {
    /* Default component class */
    class def : public controls::div
    {
    public:

      box_props StdDivBoxProps = box_props({ .MarginW = 4, .BorderW = 2, .PaddingW = 2 });
      box_props StdMarginBoxProps = box_props({ .MarginW = 2, .BorderW = 0, .PaddingW = 0 });
      controls::div_style StdDivStyle = controls::div_style({ .SpaceColor = {0.35}, .BorderColor = {0.75} });

      /* Create value function - test component */
      entity * CreateValue( const std::string &Name )
      {
        std::string Prefix = std::format("Value {} ", Name);

        return
          Create<controls::div>({
            .Id = Prefix + "main bar",
            .MinSize = size_ref::eMinContent,
            .LayoutType = layout_type::eFlexColumn,
            .BoxProps = StdDivBoxProps,
            .Style = StdDivStyle
          }, {
            Create<controls::div>({
              .Id = Prefix + "slider bar",
              .MinSize = size_ref::eMinContent,
              .LayoutType = layout_type::eFlexRow,
              .BoxProps = StdMarginBoxProps,
              .Style = StdDivStyle
            }, {
              Create<controls::text>({
                .Id = Prefix + " slider bar text",
                .Flex = { .Grow = 1 },
                .BoxProps = StdDivBoxProps,
                .Props = { .IsSingleLine = true, .Str = "Button of value " + Name },
                .Style = { .Color = vec3(1) }
              }),
              Create<controls::slider>({
                .Id = "slider bar slider",
                .Size = 30, 
                .MinSize = size_ref::eMinContent,
                .Flex = { .Basis = flex_basis_type::eMaxContent, .Grow = 1 },
                .BoxProps = StdDivBoxProps,
                .Style = {
                  .Track = { .Space = { .DefColor = vec3::Rnd0() }, .Border = { .DefColor = vec3::Rnd0() } },
                  .Thumb = { .Space = { .DefColor = vec3::Rnd0() }, .Border = { .DefColor = vec3::Rnd0() } }
                }
              }),
            }),
            Create<controls::div>({
              .Id = Prefix + "range bar",
              .MinSize = size_ref::eMinContent,
              .LayoutType = layout_type::eFlexRow,
              .BoxProps = StdMarginBoxProps,
              .Style = StdDivStyle
            }, {
              Create<controls::text>({
                .Id = Prefix + " range bar text",
                .Flex = { .Grow = 1 },
                .BoxProps = StdDivBoxProps,
                .Props = { .IsSingleLine = true, .Str = "Button of value " + Name },
                .Style = { .Color = vec3(1) }
              }),
              Create<controls::range>({
                .Id = "range bar range",
                .Size = 30, 
                .MinSize = size_ref::eMinContent,
                .Flex = { .Basis = flex_basis_type::eMaxContent, .Grow = 1 },
                .BoxProps = StdDivBoxProps,
                .Style = {
                  .Left = { .Space = { .DefColor = vec3::Rnd0() }, .Border = { .DefColor = vec3::Rnd0() } },
                  .Right = { .Space = { .DefColor = vec3::Rnd0() }, .Border = { .DefColor = vec3::Rnd0() } }
                }
              }),
            }),
            Create<controls::div>({
              .Id = Prefix + "button bar",
              .MinSize = size_ref::eMinContent,
              .LayoutType = layout_type::eFlexRow,
              .BoxProps = StdMarginBoxProps,
              .Style = StdDivStyle
            }, {
              Create<controls::text>({
                .Id = Prefix + " button bar text",
                .Flex = { .Grow = 1 },
                .BoxProps = StdDivBoxProps,
                .Props = { .IsSingleLine = true, .Str = "Button of value " + Name },
                .Style = { .Color = vec3(1) }
              }),
              Create<controls::button>({
                .Id = "second button bar button",
                //.Flex = { .Grow = 1 },
                .BoxProps = StdDivBoxProps,
                .Props = { .IsPress = true, .Str = "Click", .OnChangeCallBack = []( controls::button *Button ){ Log(std::format("New value: {}", Button->GetValue() )); } },
              }),
            }),
            // Create<controls::button>({
            //   .Id = "second button bar button",
            //   .Size = 30, 
            //   .MinSize = 30,
            //   .Flex = { .Basis = flex_basis_type::eFixed },
            //   .BoxProps = StdDivBoxProps,
            //   .Props = { .IsPress = true, .OnChangeCallBack = []( controls::button *Button ){ Log(std::format("New value: {}", Button->GetValue() )); } },
            // }),
            
          }); // Main bar
      } /* End of 'CreateValue' function */

      /* Default component class construct function */
      def( const entity_props<def> &NewProps ) :
        controls::div({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .LayoutType = layout_type::eFlexColumn,
          .Flex = NewProps.Flex,
          .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.3, 0.3, 0.75} },
        })
      {
        std::vector<::ui::entity *> Values;
    
        for (UINT i = 0; i < 5; i++)
          Values.push_back(CreateValue(std::format("{}", i)));

        AddChildren({
          Create<controls::div>({
              .LayoutType = layout_type::eFlexRow,
              .Flex = { .Grow = 1, .Shrink = 1 },
              .BoxProps = StdDivBoxProps,
              .Style = StdDivStyle
            }, {
              Create<controls::div>({
                .Size = {300},
                .LayoutType = layout_type::eFlexColumn,
                .Flex = { .Basis = flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
                .BoxProps = StdDivBoxProps,
                .Style = StdDivStyle,
              }, {
                Create<controls::div>({
                  .Size = {300},
                  .LayoutType = layout_type::eFlexColumn,
                  .Flex = { .Basis = flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
                  .BoxProps = StdDivBoxProps,
                  .Style = StdDivStyle,
                }, {
                  Create<controls::div>({
                    .MinSize = size_ref::eMinContent,
                    .LayoutType = layout_type::eFlexRow,
                    .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                    .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
                  }, {
                    Create<controls::text>({
                      .Flex = { .Grow = 1 },
                      .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                      .Props = { .IsSingleLine = true, .Str = "Show console" },
                      .Style = { .Color = vec3(1) }
                    }),
                    Create<controls::button>({
                      .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                      .Value = false,
                      .Props = {
                        .IsPress = true,
                        .Str = "Show",
                        .OnChangeCallBack = [&]( controls::button *Button ){
                          switch (Button->GetValue())
                          {
                          case true:
                            AllocConsole();
                            freopen("conin$","r",stdin);
                            freopen("conout$","w",stdout);
                            freopen("conout$","w",stderr);
                            break;
                          case false:
                            FreeConsole();
                            break;
                          }
                        },
                      },
                    }),
                  }),
                }),
                Create<controls::div>({
                  .Id = "Box 1.1",
                  .Size = {300},
                  .MaxSize = {500},
                  .LayoutType = layout_type::eFlexColumn,
                  .Overflow = overflow_type::eScroll,
                  .Flex = { .Basis = flex_basis_type::eFixed, .Grow = 1, .Shrink = 1 },
                  .BoxProps = StdDivBoxProps,
                  .Style = StdDivStyle,
                }, Values),
              }),
              Create<controls::div>({
                .Id = "Box 2",
                .Size = {300},
                .MinSize = {270},
                .MaxSize = size_ref::eNone,
                .LayoutType = layout_type::eFlexColumn,
                .Flex = { .Basis = flex_basis_type::eFixed, .Grow = 2, .Shrink = 2 },
                .BoxProps = StdDivBoxProps,
                .Style = StdDivStyle,
              }, {
                Create<components::tabs>({
                  .Id = "TabsBox",
                  .Flex = { .Grow = 1, .Shrink = 1 },
                  .Tabs = {
                    {"Tab name",
                      Create<controls::text>({
                        .Flex = { .Grow = 1, .Shrink = 1 },
                        .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                        .Props = { .Str = "These are tabs.\nYou can switch them.\nGood luck.\n"},
                        .Style = { .Color = vec3(1) }
                      }),
                    },
                    {"Car radio",
                      Create<controls::text>({
                        .Id = "True text",
                        .Flex = { .Grow = 1, .Shrink = 1 },
                        .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                        .Props = { .Str = "I pounder of something great,\nmy lungs will fill and then deflate.\nThey fill with fire, exhale desire,\nI know it's dire my time today."},
                        .Style = { .Color = vec3(1) }
                      }),
                    },
                    {"Small description",
                      Create<components::tabs>({
                        .Id = "TabsBox",
                        .Flex = { .Grow = 1, .Shrink = 1 },
                        .TabsDir = dir_type::eVertical,
                        .Tabs = {
                          {"Authors",
                            Create<controls::text>({
                              .Id = "Authors tab",
                              .Flex = { .Grow = 1, .Shrink = 1 },
                              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                              .Props = { .Str = "Fedor Borodulin\nElizaveta Sopina\nDiana Lvova\nAndrey Egorov\nTihon Chudakov\n"},
                              .Style = { .Color = vec3(1) }
                            }),
                          },
                          {"Idiology",
                            Create<controls::text>({
                              .Id = "BIG text",
                              .Flex = { .Grow = 1, .Shrink = 1 },
                              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                              .Props = { .Str = "The animation is a container that stores the interfaces of the main systems and the primitive context and provides an initialization and deinitialization call in accordance with the further description.\nEach system communicates with other systems and user logic through its own standard interface called interface. The interface has only public virtual methods without implementation and optional non-virtual public auxiliary methods that call virtual ones internally.\nSystems can exchange platform-specific data, but cannot expose it to user logic.\nThe animation architecture allows the use of parallel threads in systems, so the documentation should indicate any ambiguous rules for cross-thread use."},
                              .Style = { .Color = vec3(1) }
                            }),
                          },
                          {"Other text",
                            Create<controls::text>({
                              .Id = "Other text tab",
                              .Flex = { .Grow = 1, .Shrink = 1 },
                              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                              .Props = { .Str = "Some text"},
                              .Style = { .Color = vec3(1) }
                            }),
                          },
                        },
                      }),
                    },
                    {"Text test",
                      Create<components::text_test>({
                        .Id = "ComponetBox",
                        .Flex = { .Grow = 1, .Shrink = 1 },
                      }),
                    }
                  },
                }),
                // Create<controls::line_editor>({
                //   .Id = "Line editor",
                //   .MaxSize = {10000},
                //   .Flex = { .Grow = 0 },
                //   .BoxProps = StdDivBoxProps,
                //   .Props = { .Str = "Some text" },
                //   .Style = { .Color = vec3(1) }
                // }),
              }),
            }) // Main bar
        });
      } /* End of 'chat' function */

    }; /* End of 'chat' class */

  } /* end of 'examples' namespace */
} /* end of 'ui' namespace */

#endif // __ui_examples_default_h_