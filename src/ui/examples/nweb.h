#include <map>
#include <cstdlib>
#include "../../socket/sl.h"


#ifndef __ui_examples_nweb_h_
#define __ui_examples_nweb_h_

#include "../entity.h"

// Controls
#include "../controls/div.h"
#include "../controls/text.h"
#include "../controls/button.h"
#include "../controls/slider.h"
#include "../controls/range.h"
#include "../controls/text_editor.h"
#include "../controls/canvas.h"

#include "../frame_render.h"

// NWeb
#include "utilities/nn/nn.h"

namespace ui
{
  namespace examples
  {
    class nweb;
  } /* end of 'examples' namespace */

  template<>
    struct entity_props<examples::nweb>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
    }; /* End of 'entity_props' struct */

  namespace examples
  {
    /* nweb component class */
    class nweb : public controls::div
    {
      nn::nn Net;
      std::vector<FLT> InputLayerValues;

    public:

      /* nweb component class construct function */
      nweb( const entity_props<nweb> &NewProps ) :
        controls::div({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .LayoutType = layout_type::eFlexColumn,
          .Flex = NewProps.Flex,
          .BoxProps = { .BorderW = 2, .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.3, 0.3, 0.75} },
        })
      {

        std::vector<UINT> LayersProps = {
          2, 2, 2,
        };
        Net.Init(LayersProps);
        InputLayerValues.resize(LayersProps[0]);

        // Input values entities
        std::vector<entity *> InputEs = {};

        for (SIZE_T i = 0; i < LayersProps[0]; i++)
          InputLayerValues[i] = 0, InputEs.push_back(Create<controls::line_editor>({
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Props = {
              .Str = std::to_string(InputLayerValues[i]),
              .WidthChars = 10,
            },
          }));

        // Output entity
        controls::text *OutputE = Create<controls::text>({
          .Flex = { .Grow = 1 },
          .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
          .Props = { .Str = "no output" },
        });

        // Random values limits entities
        controls::line_editor
          *BiasLowerLimitE = Create<controls::line_editor>({
            .Props = {
              .Str = std::to_string(0),
              .WidthChars = 10,
            },
          }),
          *BiasUpperLimitE = Create<controls::line_editor>({
            .Props = {
              .Str = std::to_string(1),
              .WidthChars = 10,
            },
          }),
          *WeightLowerLimitE = Create<controls::line_editor>({
            .Props = {
              .Str = std::to_string(0),
              .WidthChars = 10,
            },
          }),
          *WeightUpperLimitE = Create<controls::line_editor>({
            .Props = {
              .Str = std::to_string(1),
              .WidthChars = 10,
            },
          }),
          *ChangeCoefInput = Create<controls::line_editor>({
            .Props = {
              .Str = std::to_string(0.1),
              .WidthChars = 10,
            },
          });

        // Main children
        AddChildren({
          Create<components::tabs>({
            .Id = "Functional tabs",
            .Flex = { .Grow = 1, .Shrink = 1 },
            .Tabs = {
              {"Main contol",
                Create<controls::div>({
                  .LayoutType = layout_type::eFlexRow,
                  .Flex = { .Grow = 1 },
                  .BoxProps = { .PaddingW = 2 },
                  .Style = { .SpaceColor = {0.45} },
                },{
                  Create<controls::div>({ // Left
                    .LayoutType = layout_type::eFlexColumn,
                    .Flex = { .Basis = flex_basis_type::eFixed, .Grow = 1 },
                    .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                    .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
                  },{
                    Create<controls::text>({
                      .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                      .Props = { .Str = "INPUT" },
                      .Style = { .LayoutFlags = (DWORD)render_2d::hor_align::eCenter }
                    }),
                    Create<controls::div>({
                      .LayoutType = layout_type::eFlexColumn,
                      .Style = { .SpaceColor = {0.35} },
                    }, InputEs),
                    Create<controls::button>({
                      .Props = {
                        .Str = "Set for input",
                        .OnClickCallBack = [OutputE, InputEs, this]( controls::button *Button ){
                          for (SIZE_T i = 0; i < InputEs.size(); i++)
                            InputLayerValues[i] = strtof((dynamic_cast<controls::line_editor *>(InputEs[i]))->GetStr().c_str(), nullptr);
                          Net.SetInput(InputLayerValues);
                        }
                      }
                    }),
                    Create<controls::button>({
                      .Props = {
                        .Str = "Set for expected",
                        .OnClickCallBack = [OutputE, InputEs, this]( controls::button *Button ){
                          std::vector<FLT> ExpectedLayerValues(InputEs.size());

                          for (SIZE_T i = 0; i < InputEs.size(); i++)
                            ExpectedLayerValues[i] = strtof((dynamic_cast<controls::line_editor *>(InputEs[i]))->GetStr().c_str(), nullptr);
                          Net.SetExpectedOutput(ExpectedLayerValues);
                        }
                      }
                    }),
                    Create<controls::button>({
                      .Props = {
                        .Str = "Submit",
                        .OnClickCallBack = [OutputE, InputEs, this]( controls::button *Button ){
                          Net.Dispatch();
                          std::vector<FLT> OutputVs = Net.GetOutput();
                          std::string OutStr = "Net output:\n";

                          for (SIZE_T i = 0; i < OutputVs.size(); i++)
                            OutStr += std::format("{}: {}\n", i, OutputVs[i]);

                          OutputE->SetStr(OutStr);
                        }
                      }
                    }),
                    Create<controls::button>({
                      .Props = {
                        .Str = "Teach",
                        .OnClickCallBack = [OutputE, this]( controls::button *Button ){
                          FLT Error = Net.Backprop();
                          
                          OutputE->SetStr(std::format("ERROR: "));
                        }
                      }
                    }),
                    Create<controls::button>({
                      .Props = {
                        .Str = "Set to random",
                        .OnClickCallBack = [BiasLowerLimitE, BiasUpperLimitE, WeightLowerLimitE, WeightUpperLimitE, this]( controls::button *Button ){
                          Net.SetToRandom(
                            strtof(BiasLowerLimitE->GetStr().c_str(), nullptr),
                            strtof(BiasUpperLimitE->GetStr().c_str(), nullptr),
                            strtof(WeightLowerLimitE->GetStr().c_str(), nullptr),
                            strtof(WeightUpperLimitE->GetStr().c_str(), nullptr)
                          );
                        },
                      }
                    }),
                    Create<controls::button>({
                      .Props = {
                        .Str = "Teach cycle",
                        .OnClickCallBack = [OutputE, this]( controls::button *Button ){
                          
                          //auto OutValues = Net.GetOutput();
                          //std::string OutStr = "Input:\n";
                          //
                          //for (auto v : InputLayerValues)
                          //  OutStr += std::to_string(v) + "\n";
                          //
                          //OutStr += "Output:\n";
                          //
                          //for (auto v : OutValues)
                          //  OutStr += std::to_string(v) + "\n";
                          //

                          std::string OutStr = "";
                          const UINT
                            CycleCount = 3,
                            Count = 100;
                          FLT ErrorSum = 0;
                          for (UINT i = 0; i < Count; i++)
                          {
                            for (UINT j = 0; j < CycleCount; j++)
                            {
                              InputLayerValues = {(FLT)rand() / RAND_MAX, (FLT)rand() / RAND_MAX};

                              BOOL t = InputLayerValues[0] > InputLayerValues[1];
                              std::vector<FLT> ExpValues {(FLT)t, 1 - (FLT)t};

                              Net.SetInput(InputLayerValues);
                              Net.SetExpectedOutput(ExpValues);
                              Net.Dispatch();
                              Net.AccumulateError();
                            }

                            FLT Error = Net.Backprop();
                            ErrorSum += Error;
                            OutStr += "Error: " + std::to_string(Error) + "\n";
                          }
                          OutStr = "Avarage error: " + std::to_string(ErrorSum / Count) + "\n" + OutStr;
                          
                          OutputE->SetStr(OutStr);
                        },
                      }
                    }),
                    Create<controls::div>({ // Bias lower
                      .LayoutType = layout_type::eFlexRow,
                    },{
                      Create<controls::text>({
                        .Flex = { .Grow = 1 },
                        .Props = { .Str = "Biases lower limit" },
                      }),
                      BiasLowerLimitE,
                    }),
                    Create<controls::div>({ // Bias upper
                      .LayoutType = layout_type::eFlexRow,
                    },{
                      Create<controls::text>({
                        .Flex = { .Grow = 1 },
                        .Props = { .Str = "Biases upper limit" },
                      }),
                      BiasUpperLimitE,
                    }),
                    Create<controls::div>({ // Weight lower
                      .LayoutType = layout_type::eFlexRow,
                    },{
                      Create<controls::text>({
                        .Flex = { .Grow = 1 },
                        .Props = { .Str = "Weights lower limit" },
                      }),
                      WeightLowerLimitE,
                    }),
                    Create<controls::div>({ // Wegith upper
                      .LayoutType = layout_type::eFlexRow,
                    },{
                      Create<controls::text>({
                        .Flex = { .Grow = 1 },
                        .Props = { .Str = "Weights upper limit" },
                      }),
                      WeightUpperLimitE,
                    }), 
                    Create<controls::div>({ // Coef
                      .LayoutType = layout_type::eFlexRow,
                    },{
                      Create<controls::text>({
                        .Flex = { .Grow = 1 },
                        .Props = { .Str = "Weights upper limit" },
                      }),
                      ChangeCoefInput,
                      Create<controls::button>({
                        .Props = {
                          .Str = "Set",
                          .OnClickCallBack = [ChangeCoefInput, this]( controls::button *Button ){
                            Net.SetChangeCoef(strtof(ChangeCoefInput->GetStr().c_str(), nullptr));
                          }
                        }
                      }),
                    }),
                  }),
                  Create<controls::div>({ // Right
                    .LayoutType = layout_type::eFlexColumn,
                    .Flex = { .Basis = flex_basis_type::eFixed, .Grow = 1 },
                    .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                    .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
                  },{
                    Create<controls::text>({
                      .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                      .Props = { .Str = "OUTPUT" },
                      .Style = { .LayoutFlags = (DWORD)render_2d::hor_align::eCenter }
                    }),
                    OutputE,

                  }),
                }),
              }
            },
          }),
          
        });
      } /* End of 'chat' function */

    }; /* End of 'chat' class */

  } /* end of 'examples' namespace */
} /* end of 'ui' namespace */

/*
 Create<controls::canvas>({
                  .MinSize = size_ref::eMinContent,
                  .Flex = { .Grow = 1 },
                  .Props = {
                    .OnDrawCallBack = [this]( const ivec2 &DrawPos, const isize2 &DrawSize, const mask &DrawMask ) {
                      for (UINT y = 0; y < DrawSize.H; y++)
                        for (UINT x = 0; x < DrawSize.W; x++)
                          Canvas->Render2d.PutPixel(DrawPos + ivec2(x, y), ToRGB(vec3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0)));
                    }
                  }
                }),
*/

#endif // __ui_examples_nweb_h_