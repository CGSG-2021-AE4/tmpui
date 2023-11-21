#ifndef __ui_components_text_test_h_
#define __ui_components_text_test_h_

#include "../entity.h"

// Controls
#include "../controls/text.h"

namespace ui
{
  namespace components
  {
    class log_output;
  } /* end of 'controls' namespace */

  template<>
    struct entity_props<components::log_output>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
    }; /* End of 'entity_props' struct */

  namespace components
  {
    /* Log output component class */
    class log_output : public controls::text
    {
    public:

      /* Text test class construct function */
      log_output( const entity_props<log_output> &NewProps ) :
        controls::text({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .Flex = NewProps.Flex,
          .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
          .Style = { .Color = ::ui::vec3(1, 1, 1) },
        })
      {
      } /* End of 'log_output' function */

      /* Log str function */
      VOID Log( const std::string &Str )
      {

      } /* End of 'Log' function */
    }; /* End of 'log_output' class */

  } /* end of 'components' namespace */
} /* end of 'ui' namespace */

#endif // __ui_components_log_output_h_