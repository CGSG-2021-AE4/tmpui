#include <map>
#include "../../socket/sl.h"


#ifndef __ui_examples_chat_h_
#define __ui_examples_chat_h_

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
    class chat;
  } /* end of 'examples' namespace */

  template<>
    struct entity_props<examples::chat>
    {
      std::string Id {""};
      ivec2 Pos {0};
      flex_props Flex {};
    }; /* End of 'entity_props' struct */

  namespace examples
  {
    /* Chat component class */
    class chat : public controls::div
    {
      controls::line_editor *IPInputLineE;
      controls::line_editor *MessageInputLineE;
      controls::div *MessagesColumnE;

      std::string UserName = "Aboba";

      ::sl::sock Port;

    public:

      /* Chat class construct function */
      chat( const entity_props<chat> &NewProps ) :
        controls::div({
          .Id = NewProps.Id,
          .Pos = NewProps.Pos,
          .LayoutType = layout_type::eFlexColumn,
          .Flex = NewProps.Flex,
          .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
          .Style = { .SpaceColor = {0.35}, .BorderColor = {0.3, 0.3, 0.75} },
        }),
        Port(::sl::sock::type::CLIENT)
      {
        AddChildren({
          Create<controls::div>({
            .MinSize = ::ui::size_ref::eMinContent,
            .LayoutType = ::ui::layout_type::eFlexRow,
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
          }, {
            Create<controls::text>({
              .Flex = { .Grow = 1 },
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .IsSingleLine = true, .Str = "Server IP: " },
              .Style = { .Color = ::ui::vec3(1) }
            }),
            IPInputLineE = Create<controls::line_editor>({
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .Str = "127.0.0.1", .WidthChars = 10 },
              .Style = { .Color = ::ui::vec3(1) }
            }),
          }),
          Create<controls::div>({
            .MinSize = ::ui::size_ref::eMinContent,
            .LayoutType = ::ui::layout_type::eFlexRow,
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
          }, {
            Create<controls::text>({
              .Flex = { .Grow = 1 },
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .IsSingleLine = true, .Str = "Connect to the server" },
              .Style = { .Color = ::ui::vec3(1) }
            }),
            Create<controls::button>({
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .IsPress = true, .Str = "Connect", .OnChangeCallBack = [&]( controls::button *Button ){
                  Port.Connect(IPInputLineE->GetStr(), 8080);
                  Log(std::format("Connected to the server {}:{}", IPInputLineE->GetStr(), 8080));
                  
                  Port.StartReader(Port.GetSocketHandle(),
                    [&]( const SOCKET hReadSocket, const std::vector<BYTE> &Message ) {
                      Canvas->WorkMutex.lock();
                      AddMessage(std::to_string(hReadSocket), std::string(Message.begin(), Message.end()));
                      Canvas->WorkMutex.unlock();
                    }
                  );
                }
              },
            }),
          }),
          MessagesColumnE = Create<controls::div>({
            .LayoutType = ::ui::layout_type::eFlexColumn,
            .Overflow = overflow_type::eScroll,
            .Flex = { .Grow = 1 },
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
          }),
          Create<controls::div>({
            .LayoutType = ::ui::layout_type::eFlexRow,
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Style = { .SpaceColor = {0.35}, .BorderColor = {0.75} },
          }, {
            MessageInputLineE = Create<controls::line_editor>({
              .Flex = { .Grow = 1 },
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = {
                .Str = "Message text",
                .WidthChars = 10,
                .OnEnterCallBack = [&]( controls::line_editor *Button ) {
                  // Send message
                  std::string Str = MessageInputLineE->GetStr();
                  
                  if (Str.empty())
                    return;

                  Send(Str);
                  MessageInputLineE->SetStr("");
                }
              },
              .Style = { .Color = ::ui::vec3(1) }
            }),
            Create<controls::button>({
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = {
                .IsPress = true,
                .Str = "Push",
                .OnChangeCallBack = [&]( controls::button *Button ) {
                  // Send message
                  std::string Str = MessageInputLineE->GetStr();
                  
                  if (Str.empty())
                    return;

                  Send(Str);
                  MessageInputLineE->SetStr("");
                }
              },
            }),
          }),
        });
      } /* End of 'text_test' function */

      /* Send message function */
      VOID Send( const std::string &MessageStr )
      {
        Port.Send(Port.GetSocketHandle(), {MessageStr.begin(), MessageStr.end()});
        AddMessage(UserName, MessageStr, true);
      } /* End of 'Send' function */

      /* Log function */
      VOID Log( const std::string &MessageStr )
      {
        MessagesColumnE->AddChild(
          Create<controls::text>({
            .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
            .Props = { .Str = MessageStr },
            .Style = { .Color = ::ui::vec3(1) }
          }));
      } /* End of 'Log' function */

      /* Add message function */
      VOID AddMessage( const std::string &UserName, const std::string &MessageStr, BOOL IsMine = false )
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
                .LayoutFlags = IsMine ? (DWORD)::ui::render_2d::hor_align::eRight : (DWORD)::ui::render_2d::hor_align::eLeft,
                .IsEmptyBack = true,
              }
            }),
            Create<controls::text>({ // Main text
              .BoxProps = { .MarginW = 4, .BorderW = 2, .PaddingW = 2 },
              .Props = { .Str = MessageStr },
              .Style = { .Color = ::ui::vec3(1), .IsEmptyBack = true }
            }),
          })
        );
      } /* End of 'AddMessage' function */

    }; /* End of 'chat' class */

  } /* end of 'examples' namespace */
} /* end of 'ui' namespace */

#endif // __ui_examples_chat_h_