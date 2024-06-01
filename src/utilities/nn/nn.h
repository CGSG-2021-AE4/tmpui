/* Neural network main handle file */
#ifndef __nn_h_
#define __nn_h_

#include "def.h"

/* Neural nerwork namespace */
namespace nn
{
  /* Layer struct */
  struct layer
  {
    std::vector<FLT>
      Zs {},      // Not normalized values
      As {},      // Values
      Biases {},  // Biases
      Weights {}, // *There is no weight at the last layer
      DCByDA {};  // Derivetives of C0 by A
  }; /* End of 'layer' struct */

  /* Neural nerwork main class */
  class nn
  {
    std::vector<layer> Layers {};          // All layers
    std::vector<FLT>
      ExpectedLastLayer {}, // Expected last layer values
      Errors {};            // Out errors
    FLT ChangeCoef = 0.1; // const TMP - coef of teaching

    /* Normalize function.
     * ARGUMENTS:
     *   - input value:
     *       FLT InputValue;
     * RETURNS:
     *   (FLT) normalized value.
     * NOTE: Currently it is sigmoid.
     */
    FLT NormalizeF( FLT InputValue );

    /* Derivetive of normalize function.
     * ARGUMENTS:
     *   - input value:
     *       FLT X;
     * RETURNS:
     *   (FLT) derivetive value.
     */
    FLT DerNormalizeF( FLT X );
    
  public:

    /* Init function.
     * ARGUMENTS:
     *   - number of neurons per layer:
     *       const std::vector<UINT> &LayersProps;
     * RETURNS: None.
     */
    VOID Init( const std::vector<UINT> &LayersProps );

    /* Close function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID );

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
    VOID SetToRandom( FLT BiasLowerLimit, FLT BiasUpperLimit, FLT WeightLowerLimit, FLT WeightUpperLimit );

    /* Set first layer(input) function.
     * ARGUMENTS:
     *   - new first layer values:
     *       const std::vector<FLT> &FirstLayer;
     * RETURNS: None.
     */
    VOID SetInput( const std::vector<FLT> &FirstLayer );

    /* Set change coef function.
     * ARGUMENTS:
     *   - new coef:
     *       FLT NewCoef;
     * RETURNS: None.
     */
    VOID SetChangeCoef( FLT NewCoef );

    /* Main calculate function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Dispatch( VOID );

    /* Get last layer(output) function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::vector<FLT>) last layer values.
     */
    std::vector<FLT> GetOutput( VOID );

    /* Accumulate error function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID AccumulateError( VOID );

    /* Set expected output function.
     * ARGUMENTS:
     *   - expected last layer:
     *       const std::vector<FLT> &NewExpectedLastLayer;
     * RETURNS: None.
     */
    VOID SetExpectedOutput( const std::vector<FLT> &NewExpectedLastLayer );
    
    /* Compute error function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (FLT) out error.
     */
    FLT Backprop( VOID );

  }; /* End of 'nn' class */
} /* end of 'nn' namespace */
#endif // __nn_h_

/* END OF 'nn.h' FILE */
