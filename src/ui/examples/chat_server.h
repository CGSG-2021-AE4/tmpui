#include <map>
#include <mutex>
#include "../../socket/sl.h"


#ifndef __ui_examples_chat_server_h_
#define __ui_examples_chat_server_h_

#include "../entity.h"

// Controls
#include "../controls/div.h"
#include "../controls/text.h"
#include "../controls/button.h"
#include "../controls/text_editor.h"

#include "../frame_render.h"

namespace ui
{
  namespace examples
  {
    class chat_server;
  } /* end of 'examples' namespace */

  template<>
    struct entity_props<examples::chat_server>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
    }; /* End of 'entity_props' struct */

  namespace examples
  {
    /* Chat server component class */
    class chat_server : public controls::div
    {
      controls::text *LogOutputE;
      controls::div *MessagesColumnE;

      std::string UserName = "Aboba";

      ::sl::sock Port;

    public:

      /* Chat server class construct function */
      chat_server( const entity_props<chat_server> &NewProps ) :
        controls::div({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .LayoutType = ::ui::layout_type::eFlexRow,
          .Flex = NewProps.Flex,
          .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.3, 0.3, 0.75} },
        }),
        Port(::sl::sock::type::SERVER)
      {
        AddChildren({
          Create<controls::div>({
            .Size = 0,
            .LayoutType = ::ui::layout_type::eFlexColumn,
            .Overflow = overflow_type::eScroll,
            .Flex = { .Grow = 1 },
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
          }, {
            Create<controls::div>({
              .MinSize = ::ui::size_ref::eMinContent,
              .LayoutType = ::ui::layout_type::eFlexRow,
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
            }, {
              Create<controls::text>({
                .Flex = { .Grow = 1 },
                .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                .Props = { .IsSingleLine = true, .Str = "Start server" },
                .Style = { .Color = ::ui::vec3(1) }
              }),
              Create<controls::button>({
                .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
                .Props = {
                  .IsPress = true, .Str = "Start",
                  .OnChangeCallBack = [&]( controls::button *Button ){
                    Port.Bind(8080);
                    Port.StartAcceptor(100,
                      [&]( const SOCKET ClientSocket ) {
                        Canvas->WorkMutex.lock();
                        Log("New user: " + std::to_string(ClientSocket));
                        Canvas->WorkMutex.unlock();
                      },
                      [&]( const SOCKET hReadSocket, const std::span<BYTE> &Message ) {
                        if (Message.empty())
                          return;
                        Canvas->WorkMutex.lock();
                        Log("New message: " + std::string(Message.begin(), Message.end()));
                        Canvas->WorkMutex.unlock();
                        Port.SendToAllExcept(hReadSocket, Message);
                      }
                    );
                  }
                },
              }),
            }),
          }),
          LogOutputE = Create<controls::text>({
            .Size = 0,
            .Flex = { .Basis = flex_basis_type::eFixed, .Grow = 3 },
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Props = { .Str = UserName }, // Align to right if not me TODO
            .Style = { .Color = ::ui::vec3(1, 0, 1) }
          }),
        });
        Port.Log = [&]( const std::string &Str )
        {
          Log("SOCKET: " + Str);
        };
      } /* End of 'text_test' function */

      /* Log to output function */
      VOID Log( const std::string &Str )
      {
        if (Str.empty())
          return;
        LogOutputE->SetStr(LogOutputE->Str.WholeStr + (!LogOutputE->Str.WholeStr.empty() ? "\n" : "") + Str);
      } /* End of 'Log' function */

      /* Add message function */
      VOID AddMessage( const std::string &UserName, const std::string &MessageStr, BOOL IsMine )
      {
        MessagesColumnE->AddChild(
          ::ui::Create<controls::div>({
            .LayoutType = ::ui::layout_type::eFlexColumn,
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} }
          }, {
            Create<controls::text>({ // Username
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .Str = UserName }, // Align to right if not me TODO
              .Style = {
                .Color = ::ui::vec3(1, 0, 1),
                .LayoutFlags = (DWORD)::ui::render_2d::hor_align::eRight,
              }
            }),
            Create<controls::text>({ // Main text
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .Str = MessageStr },
              .Style = { .Color = ::ui::vec3(1) }
            }),
          })
        );
      } /* End of 'AddMessage' function */

    }; /* End of 'chat_server' class */

  } /* end of 'examples' namespace */
} /* end of 'ui' namespace */

#endif // __ui_examples_chat_h_