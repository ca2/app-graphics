#include "platform/os/metrowin.h"



[MTAThread]
int main(Array<String^>^)
{

   auto source = ::metrowin::new_directx_application_source("m_app.exe : app=app-core/hellomultiverse");

   ::winrt::Windows::ApplicationModel::Core::CoreApplication::Run(source);
   
   return 0;

}



/*[MTAThread]
int main(Array<String^>^)
{
   
   // No Depends
   
   return 0;

}
*/
