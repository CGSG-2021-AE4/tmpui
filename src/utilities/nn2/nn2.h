/* Neural network second version main handle file */
#ifndef __nn2_h_
#define __nn2_h_

#include "def.h"

// Neural nerwork namespace
namespace nn2
{
  // Architecture descriptor
  struct net_arc
  {
    std::vector<UINT> Layers;
  }; // End of 'net_arc' struct

  // Net configuration class - all biases, weights, etc.
  class net_config
  {
  public:

    std::vector<std::vector<FLT>> Biases {};
    std::vector<std::vector<std::vector<FLT>>> Weights {};

    // Default empty constructor
    net_config( VOID )
    {
    } // End of 'net_config' function

    // Constructor from architecture with filling with 0
    net_config( const net_arc &NetArc )
    {
      Biases.resize(NetArc.Layers.size());
      Weights.resize(NetArc.Layers.size() - 1);
      
      for (SIZE_T i = 0; i < NetArc.Layers.size(); i++)
      {
        Biases[i].resize(NetArc.Layers[i]);
        if (i != NetArc.Layers.size() - 1)
        {
          Weights[i].resize(NetArc.Layers[i]);
          for (SIZE_T j = 0; j < Weights[i].size(); j++)
            Weights[i][j].resize(NetArc.Layers[i + 1]);
        }
      }
    } // End of 'net_config' function

    // Constructor from given values
    net_config( const std::vector<std::vector<FLT>> &Biases, const std::vector<std::vector<std::vector<FLT>>> &Weights ) :
      Biases(Biases),
      Weights(Weights)
    {
    } // End of 'net_config' function

    // Fill with random values
    VOID Fill( const std::pair<FLT, FLT> &BiasesRange, const std::pair<FLT, FLT> &WeightsRange )
    {
      FLT BiasesRangeL = abs(BiasesRange.second - BiasesRange.first);
      FLT WeightsRangeL = abs(WeightsRange.second - WeightsRange.first);

      // Biases
      for (auto &l : Biases)
        for (auto &n : l)
          n = ((FLT)rand() / RAND_MAX) * BiasesRangeL + BiasesRange.first;
      // Weights
      for (auto &l : Weights)
        for (auto &ni : l)
          for (auto &nj : ni)
            nj = ((FLT)rand() / RAND_MAX) * WeightsRangeL + WeightsRange.first;
    } // End of 'Fill' function
  }; // End of 'arc_config' struct

  // Main class for computing and learning
  class net_machine
  {
  private:

    net_config Config {};                    // Current config
    std::vector<std::vector<FLT>> Values {}; // Current neurons values
    // Errors
    std::vector<std::vector<FLT>> BiasErrors {}; // Accamulates bias errors
    std::vector<std::vector<std::vector<FLT>>> WeightsErrors {}; // Accamulates weights errors

    // Forbidding copy and move
    net_machine( const net_machine & ) = delete;
    net_machine & operator=( const net_machine & ) = delete;
    net_machine( net_machine && ) = delete;
    net_machine & operator=( net_machine && ) = delete;

  public:

    // Default constructor
    net_machine( VOID )
    {
    } // End of 'net_machine' function

    // Setup with a new config
    VOID Setup( const net_config &NewConfig )
    {
      Config = NewConfig;

      // Resizing values and errors
      Values.resize(NewConfig.Biases.size());
      BiasErrors.resize(NewConfig.Biases.size());
      WeightsErrors.resize(NewConfig.Weights.size());
      for (SIZE_T i = 0; i < Values.size(); i++)
      {
        Values[i].resize(NewConfig.Biases[i].size());
        BiasErrors[i].resize(NewConfig.Biases[i].size());
      }
      for (SIZE_T i = 0; i < NewConfig.Weights.size(); i++)
      {
        WeightsErrors[i].resize(NewConfig.Weights[i].size());
        for (SIZE_T j = 0; j < NewConfig.Weights[i].size(); j++)
          WeightsErrors[i][j].resize(NewConfig.Weights[i][j].size());
      }
      Reset();
    } // End of 'Setup' function

    // Get current config - at least for saving
    const net_config & GetConfig( VOID ) const
    {
      return Config;
    } // End of 'GetConfig' function

    // Clear value - set to 0
    VOID ClearValues( VOID )
    {
      for (auto &l : Values)
        for (auto &n : l)
          n = 0;
    } // End of 'ClearValues' function

    // Clear error - set to 0
    VOID ClearErrors( VOID )
    {
      for (auto &l : BiasErrors)
        for (auto &n : l)
          n = 0;
      for (auto &l : WeightsErrors)
        for (auto &n1 : l)
          for (auto &n2 : n1)
            n2 = 0;
    } // End of 'ClearErrors' function

    // Reset - clear whole dynamic state
    VOID Reset( VOID )
    {
      ClearValues();
      ClearErrors();
    } // End of 'Reset' function

    // Set first layer - input
    BOOL SetInput( const std::vector<FLT> &FirstL )
    {
      if (Values[0].size() != FirstL.size())
        return FALSE;

      Values[0] = FirstL;
      return TRUE;
    } // End of 'SetInput' function

    // Get last layer - output
    const std::vector<FLT> & GetOutput( VOID ) const
    {
      return Values.back();
    } // End of 'GetOutput' function

    /******** Computing and learning functions *********/
    
    // Normalize neuron value - currently it is sigmoid
    FLT NormalizeValue( const FLT Value ) const
    {
      return 1 / (1 + exp(-Value));
    } // End of 'NormalizeValue' function

    // Derivetive of neuron value normalize function
    FLT DerivetiveNormalizeValue( const FLT Value ) const
    {
      const FLT Exp = exp(-Value);

      return Exp / pow(1 + Exp, 2);
    } // End of 'NormalizeValue' function

    // Run the input through the net
    VOID Dispatch( VOID )
    {
      for (SIZE_T li = 0; li < Values.size(); li++)
      {
        for (SIZE_T ni = 0; ni < Values[li].size(); ni++)
          Values[li][ni] = NormalizeValue(Values[li][ni] + Config.Biases[li][ni]);

        if (li != Values.size() - 1) // OPTIMIZE
          for (SIZE_T ni1 = 0; ni1 < Values[li].size(); ni1++)
            for (SIZE_T ni2 = 0; ni2 < Values[li + 1].size(); ni2++)
              Values[li + 1][ni2] += Values[li][ni1] * Config.Weights[li][ni1][ni2];
      }
    } // End of 'Dispatch' function

    // Calculates errors by backpropagation algirithm and accamulate them
    FLT ComputeErrors( const std::vector<FLT> ExcpectedOuput )
    {
      // Get output errors
      std::vector<FLT> Errors {};

      Errors.resize(Values.back().size());
      for (SIZE_T i = 0; i < Errors.size(); i++)
        Errors[i] = ExcpectedOuput[i] - Values.back()[i];
      // Backpropagation
      for (SIZE_T li = Values.size() - 1; li != 0; li--)
      {

      }
      
      // Compute result error
    } // End of 'ComputeErrors' function
    
    // Apply errors to config values
    VOID ApplyErrors( const FLT ApplyCoef )
    {
    } // End of 'ApplyErrors' function

  }; // End of 'net_machine' class
} // end of 'nn2' namespace
#endif // __nn2_h_

/* END OF 'nn2.h' FILE */
