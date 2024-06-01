/* Neural network main source file */

#include "nn.h"

/* Neural nerwork namespace */
namespace nn
{
  /* Normalize function.
   * ARGUMENTS:
   *   - input value:
   *       FLT X;
   * RETURNS:
   *   (FLT) normalized value.
   * NOTE: Currently it is sigmoid.
   */
  FLT nn::NormalizeF( FLT X )
  {
    // return X / (1 + abs(X));
    return 1 / (1 + exp(-X));
  } /* End of 'nn::NormalizeF' function */

  /* Derivetive of normalize function.
   * ARGUMENTS:
   *   - input value:
   *       FLT X;
   * RETURNS:
   *   (FLT) derivetive value.
   */
  FLT nn::DerNormalizeF( FLT X )
  {
    const FLT Exp = exp(-X);

    return Exp / pow(1 + Exp, 2);
  } /* End of 'nn::DerNormalizeF' function */

  /* Init function.
   * ARGUMENTS:
   *   - number of neurons per layer:
   *       const std::vector<UINT> &LayersProps;
   * RETURNS: None.
   */
  VOID nn::Init( const std::vector<UINT> &LayersProps )
  {
    if (Layers.size() > 0) // It is already inited
      Close(); // Deinit

    Layers = std::vector<layer>(LayersProps.size());

    for (SIZE_T i = 0; i < LayersProps.size(); i++)
    {
      Layers[i].Zs.resize(LayersProps[i]);
      Layers[i].As.resize(LayersProps[i]);
      Layers[i].Biases.resize(LayersProps[i]);
      Layers[i].DCByDA.resize(LayersProps[i]);

      if (i != LayersProps.size() - 1) // Don't have to set last weights
        Layers[i].Weights.resize(LayersProps[i] * LayersProps[i + 1]);
      else
        Layers[i].Weights = {};
    }

    Errors.resize(Layers.back().As.size());
  } /* End of 'nn::Init' function */

  /* Close function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID nn::Close( VOID )
  {
    Layers.clear();
    ExpectedLastLayer.clear();
    Errors.clear();
  } /* End of 'nn::Close' function */

  /* Set layers to random values function.
   * ARGUMENTS:
   *   - bias lower limit:
   *       FLT BiasLowerLimit;
   *   - bias upper limit:
   *       FLT BiasUpperLimit;
   *   - weight lower limit:
   *       FLT WeightLowerLimit;
   *   - weight upper limit:
   *       FLT WeightUpperLimit;
   * RETURNS: None.
   */
  VOID nn::SetToRandom( FLT BiasLowerLimit, FLT BiasUpperLimit, FLT WeightLowerLimit, FLT WeightUpperLimit )
  {
    FLT
      BiasRange = BiasUpperLimit - BiasLowerLimit,
      WeightRange = WeightUpperLimit - WeightLowerLimit;

    for (auto &Layer : Layers)
    {
      for (auto &Bias : Layer.Biases)
        Bias = ((FLT)rand() / RAND_MAX) * BiasRange;
      for (auto &Weight : Layer.Weights)
        Weight = ((FLT)rand() / RAND_MAX) * WeightRange;
    }
  } /* End of 'nn::SetToRandom' function */

  /* Set first layer(input) function.
   * ARGUMENTS:
   *   - new first layer values:
   *       const std::vector<FLT> &FirstLayer;
   * RETURNS: None.
   */
  VOID nn::SetInput( const std::vector<FLT> &FirstLayer )
  {
    if (FirstLayer.size() == Layers[0].Zs.size())
      Layers[0].Zs = FirstLayer;
  } /* End of 'nn::SetInput' function */

  /* Set change coef function.
   * ARGUMENTS:
   *   - new coef:
   *       FLT NewCoef;
   * RETURNS: None.
   */
  VOID nn::SetChangeCoef( FLT NewCoef )
  {
    ChangeCoef = NewCoef;
  } /* End of 'nn::SetChangeCoef' function */

  /* Main calculate function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID nn::Dispatch( VOID )
  {
    for (auto NextL = Layers.begin(), CurL = NextL++; NextL != Layers.end(); ++CurL, ++NextL)
    {
      for (FLT &Z : NextL->Zs)
        Z = 0;

      for (SIZE_T CurLAi = 0; CurLAi < CurL->As.size(); CurLAi++)
      {
        const SIZE_T CurWeigthOffset = CurLAi * NextL->As.size();

        CurL->As[CurLAi] = NormalizeF(CurL->Zs[CurLAi]);
        for (SIZE_T NextLAi = 0; NextLAi < NextL->As.size(); NextLAi++)
          NextL->Zs[NextLAi] += CurL->As[CurLAi] * CurL->Weights[CurWeigthOffset + NextLAi] + NextL->Biases[NextLAi];
      }
    }
    for (SIZE_T i = 0; i < Layers.back().As.size(); i++) // The last layer
      Layers.back().As[i] = NormalizeF(Layers.back().Zs[i]); // !!! SHIT
  } /* End of 'nn::Dispatch' function */

  /* Get last layer(output) function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (std::vector<FLT>) last layer values.
   */
  std::vector<FLT> nn::GetOutput( VOID )
  {
    return Layers.back().As;
  } /* End of 'nn::GetOutput' function */

  /* Accumulate error function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID nn::AccumulateError( VOID )
  {
    for (SIZE_T i = 0; i < Errors.size(); i++)
      Errors[i] += Layers.back().As[i] - ExpectedLastLayer[i];
  } /* End of 'nn::AccumulateError' function */

  /* Set expected output function.
   * ARGUMENTS:
   *   - expected last layer:
   *       const std::vector<FLT> &NewExpectedLastLayer;
   * RETURNS: None.
   */
  VOID nn::SetExpectedOutput( const std::vector<FLT> &NewExpectedLastLayer )
  {
    if (NewExpectedLastLayer.size() == Layers.back().As.size())
      ExpectedLastLayer = NewExpectedLastLayer;
  } /* End of 'nn::SetExpectedOutput' function */
  
  /* Backpropagation function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (FLT) out error.
   */
  FLT nn::Backprop( VOID )
  {
    for (SIZE_T i = 0; i < Errors.size(); i++)
    {
      Layers.back().DCByDA[i] = 2 * Errors[i];
      Layers.back().Biases[i] -= Layers.back().DCByDA[i] * DerNormalizeF(Layers.back().Biases[i]) * ChangeCoef; // !!! CHANGE bias
    }

    for (INT Li = (INT)Layers.size() - 2; Li >= 0; Li--)
    {
      for (SIZE_T i = 0; i < Layers[Li].As.size(); i++)
      {
        Layers[Li].DCByDA[i] = 0;

        for (SIZE_T j = 0; j < Layers[Li + 1].As.size(); j++)
        {
          Layers[Li].DCByDA[i] += pow(Layers[Li + 1].DCByDA[j] * DerNormalizeF(Layers[Li + 1].Biases[j]) * Layers[Li].Weights[i * Layers[Li + 1].As.size() + j], 2);
          Layers[Li].Weights[i * Layers[Li + 1].As.size() + j] -= Layers[Li + 1].DCByDA[j] * DerNormalizeF(Layers[Li + 1].Biases[j]) * Layers[Li + 1].As[i] * ChangeCoef; // !!! CHANGE weight
        }
        Layers[Li].Biases[i] -= Layers[Li].DCByDA[i] * DerNormalizeF(Layers[Li].Biases[i]) * ChangeCoef; // !!! CHANGE bias
      }
    }

    FLT OutError = 0;

    for (SIZE_T i = 0; i < Errors.size(); i++)
    {
      OutError += pow(Errors[i], 2);
      Errors[i] = 0; // !!! SHIT
    }

    return OutError;
  } /* End of 'nn::Backprop' function */
} /* end of 'nn' namespace */

/* END OF 'nn.cpp' FILE */
