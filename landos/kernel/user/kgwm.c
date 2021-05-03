/*
 * File: kgwm.c
 * 
 *     kgwm - kernel gramado window manager.
 * 
 * History:
 *     2017 -  Created by Fred Nora.
 */

// Window manager into the kernel base.
 
// #obs:
// Aplicativo poder�o se conectar com o servidor de recusros
// gr�ficos, principalmente a GUI, que ser� chamada de 
// Gramado Window Manager.
// Outros window managers poder�o ser criados, e eles precisar�o 
// ser registrados e abrirem o servidor para usarem seus recursos.
// do mesmo modo o shell atual poder� se registrar,
// o processo de logon poder� se registrar em um servidor de logon.


#include <kernel.h>



// unsigned char  gws_bame[] = "GWS - Gramado Window Server";
// unsigned char  GramadoName[] = "GRAMADO LAYER";

//Status do gwm.
//Qual � o id do processo que � o atual window manager.
//o oficial � o Gramado Window Manager, hoje 
//chamado de shell.bin
//int gws_status;
//int gws_wm_PID;
//int gws_wm_status;


int kgwm_status;
int kgwm_wm_PID;
int kgwm_wm_status;



//
// # internal
//

void gui_create_screen (void);
void gui_create_background (void);
void gui_create_logo (void);
void gui_create_taskbar (void);
void gui_create_mainwindow (void);
void gui_create_controlmenu (void);
void gui_create_infobox (void);
void gui_create_messagebox (void);
void gui_create_debug (void);
void gui_create_navigationbar (void);
void gui_create_grid (void);


// =======================================

// #bugbug
// A fun��o diz que estamos tratando do ws, mas a flag afeta o wm.
int gwsOpen (void)
{
    kgwm_status = TRUE;
    return 0;
}

// #bugbug
// A fun��o diz que estamos tratando do ws, mas a flag afeta o wm.
int gwsClose (void)
{
    kgwm_status = FALSE;
    return 0;
}


// Registrar um window manager.
int kgwmRegisterWindowManager ( int pid )
{
    int Status = 0;

    if ( pid < 0 ){
        printf("kgwmRegisterWindowManager: pid\n");
    }

    if ( kgwm_status != TRUE ){
        Status = 1;
        goto fail;
    }else{
        kgwm_wm_PID = (int) pid;
        kgwm_wm_status = TRUE;
        return 0;
    };

fail:
    printf("kgwmRegisterWindowManager: fail\n");
    return Status;
}


/*
 * SetGuiParameters:
 *     Configura a inicializa��o das janelas gerenciadas pelo kernel.
 *     @todo: Limite de janelas (+- 10)
 *
 */
 
void 
SetGuiParameters(
    int refresh,        //Flag. Colocar ou n�o o backbuffer na tela.
    int screen,         //Tela do computador.
    int background,     //Background da area de trabalho. 
    int logo,           //Logo, Janela para imagem da �rea de trabalho.
    int taskbar,        //Barra de tarefas.(�cones para programas).
    int main,           //Janela principal.(Desktop, ou Kernel Setup Utility). 
    int menu,           //Control menu da �rea de trabalho.
    int infobox,        //Janelinha no canto, para alerta sobre sobre eventos ocorridos no sistema.
    int messagebox,     //O message box do kernel.(Cada app cria seu messagebox). 
    int debug,          //Janela do debug do kernel.(sonda por problemas).
    int navigationbar,  //Janela de navega��o(Semelhante a navigation bar dos dispositivos mobile). 
    int grid )          //Grid da janela principal.
{ 

    //Passados via argumento.
	
	//
	//@todo: gui status. fail or not.
	//
	
	// Checa validade da estrutura.

    if ( (void *) gui == NULL ){
        debug_print ("SetGuiParameters: [FAIL] gui\n");
        return;
    }else{
        gui->refresh          = refresh; 
        gui->screenStatus     = screen;
        gui->backgroundStatus = background; 
        gui->mainStatus       = main;
        gui->logoStatus       = logo; 
        gui->taskbarStatus    = taskbar;
        gui->menuStatus       = menu;
        gui->infoboxStatus    = infobox;
        gui->messageboxStatus = messagebox;
        gui->debugStatus      = debug;
        gui->navigationbarStatus = navigationbar; 
        gui->gridStatus = grid;
        // ...
    };


    //
    // #todo: 
    // More ?!!
    //
}


/*
 *****************************************
 * gui_create_screen:
 *     Cria a tela. (Screen)
 *     Atribuindo as dimens�es.
 *     ...
 * #todo: 
 * Cria buffer dedicado.
 */

void gui_create_screen (void){

    struct window_d  *hWindow; 


    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();


	// #important: 
	// N�o tem Parent Window!

    if ( (void *) gui == NULL ){
        debug_print ("gui_create_screen: [FAIL] gui\n");
        return;
    }

	//Window:
	//Pintado uma janela simples, toda preta, do tamanho da tela 
	//do sispositivo.

    hWindow = (void *) CreateWindow ( 
                           1, 0, VIEW_MINIMIZED, "Screen",
                           Left, Top, Width, Height,
                           0, 0, 0, COLOR_BLACK );

    if ( (void *) hWindow == NULL ){
         panic ("gui_create_screen: [FAIL] hWindow\n");
    }else{
        RegisterWindow (hWindow);
        kgwmSetActiveWindow (hWindow); 

        // Isso impede que rotinas mudem as caracter�scicas 
        // da janela principal 
        // sem antes destravar ou sem ter autoriza��o para isso.

        windowLock (hWindow); 

		// Estrutura gui.
        if ( (void *) gui != NULL ){
            gui->screen = (void *) hWindow;
        }

        //Desktop.
        //a janela pertence ao desktop 0
        //hWindow->desktop = (void*) desktop0;

		//Nothing.
    };

	// @todo: 
	// More ??!!

    // #bugbug: 
	// #importante:
	// N�o usar set focus nessa que � a primeira janela.
}


/*
 * gui_create_background:
 *     Cria o background. Pinta ele de uma cor.
 *     O background � filha da janela screen.
 *
 * Criando um pano de fundo do mesmo tamaho da tela,
 * Penso que � nessa janela que podemos carregar uma imagem de 
 * pano de fundo.
 *
 * #todo: 
 * Cria buffer dedicado.
 */

// #todo
// Change the return type to 'int'.

void gui_create_background (void){
 
    struct window_d  *hWindow; 

    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();


    if ( (void *) gui == NULL ){
        panic ("gui_create_background: [FAIL] gui\n");
    }


	//Window:
	//0x00808000 (verde)
	//Criando um pano de fundo do mesmo tamaho da tela,
	//Penso que � nessa janela que podemos carregar uma imagem de 
	//pano de fundo.

    hWindow = (void *) CreateWindow ( 
                           WT_SIMPLE, 0, VIEW_MINIMIZED, "Background", 
                           Left, Top, Width, Height,
                           gui->screen, 0, 0, 0x00008080 );

    if ( (void *) hWindow == NULL ){
        panic ("gui_create_background: hWindow\n");
    }

    if ( hWindow->used != TRUE || hWindow->magic != 1234 )
    {
        panic ("gui_create_background: hWindow validation\n");
    }

    // Register

    RegisterWindow (hWindow);

    // Activate.
    // ?? Is this right ??

    kgwmSetActiveWindow (hWindow); 
    
    // Set focus

    kgwmSetFocus (hWindow);

    // Lock
    
    windowLock (hWindow);  


    // Save the pointe in the main structure.
    
    if ( (void *) gui != NULL )
    {
        gui->background = (void *) hWindow;
    }

    // Desktop.
    // a janela pertence ao desktop 0
    // hWindow->desktop = (void*) desktop0; 
}


/*
 *********************************************************
 * gui_create_taskbar:
 *      Cria a taskbar da �rea de trabalho. gui->taskbar.
 *      Cria somente a janela que ser� uada pelo file manager.
 * e que servir� de refer�ncia para a janela gui->main
 */

void gui_create_taskbar (void)
{

    struct window_d *hWindow; 


    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();

    if ( (void *) gui == NULL ){
        debug_print ("gui_create_taskbar: gui\n");
        return;
    }

    // draw bar

    Height = (Height/8);

    hWindow = (void *) CreateWindow ( 
                           1, 0, VIEW_MINIMIZED, "taskbar", 
                           Left, Top, Width, Height, 
                           gui->screen, 0, 0, COLOR_WHITE );

    if ( (void *) hWindow == NULL){
        panic ("gui_create_taskbar: [FAIL] \n");
    }else{
        RegisterWindow (hWindow);
        windowLock (hWindow); 

        if ( (void *) gui != NULL )
        {
            gui->taskbarStatus = TRUE;
            gui->taskbar       = (void *) hWindow;
        }

		//...
    };

	// ...
}


/*
 ***********************************************
 * gui_create_mainwindow:
 *      A �rea de trabalho.
 *
 *      *Importante: � a �rea dispon�vel na tela para o aplicativo. 
 *      Onde ficam os �cones.
 *      
 *     A �rea de trabalho � toda a tela menos a GMB(Global Menu Bar) e 
 * menos a TaskBar. 
 * #todo: 
 * Criar o buffer dedicado para a janela principal.
 *
 * #Obs: 
 * Essa janela � especial e usar� o Backbuffer como buffer dedicado.
 */

void gui_create_mainwindow (void){

    struct window_d *hWindow; 


    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth();
    unsigned long Height = (unsigned long) screenGetHeight();


	//estrutura gui.
    if ( (void *) gui == NULL ){
        panic ("gui_create_mainwindow: [FAIL] gui\n");
    }

	//janela taskbar.
    if ( (void *) gui->taskbar == NULL ){
        panic ("gui_create_mainwindow: [FAIL] taskbar\n");
    }


	//
	// Usando a janela taskbar como refer�ncia para criar a janela main.
	//

	//Top = gui->taskbar->bottom;


	//Window:
	//Color? (?? Nem precisa pintar, mas precisa criar ??)
	//�rea de trabalho.
	//Um ret�ngulo que pode ser menor que o tamanho da tela do 
	//dispositivo.
	//� onde ficam as janelas dos aplicativos.
	//A janela principal perence ao desktop

    hWindow = (void *) CreateWindow ( 
                           1, 0, VIEW_MINIMIZED, "desktop window", 
                           Left, Top, Width, Height,           
                           gui->screen, 0, 0, COLOR_WHITE );   

    if ( (void *) hWindow == NULL){
        panic ("gui_create_mainwindow: [FAIL] hWindow\n");
     }else{   
        RegisterWindow (hWindow);
        windowLock (hWindow); 
        kgwmSetActiveWindow (hWindow); 

        //a janela pertence ao desktop 0
        //hWindow->desktop = (void*) desktop0;

        if ( (void *) gui == NULL ){
            panic ("gui_create_mainwindow: [FAIL] gui\n");
            //return;
        }else{
            gui->main = (void *) hWindow;
        };
    };



	//  Desktop Window:
	//      Criar a janela gui->desktop.
	//      Na verdade a janela do desktop
	//      � a janela da �rea de trabalho.
	// @todo:
	// #bugbug: Confunde-se com a estrutura de desktop_d.


    /*
	gui->desktop = (void*) CreateWindow( 1, 0, VIEW_MINIMIZED, "Desktop Window", 
	                                0, 16, 800, 600-16-16, 
							        gui->screen, 0, 0, COLOR_WHITE ); 
	*/


	gui->desktop = (void *) gui->main;

    if ( (void *) gui->desktop == NULL)
    {
        gui->desktop = (void *) gui->screen;

        if ( (void *) gui->desktop == NULL )
        { 
            kgwmSetFocus (hWindow);
            return; 
        }
    }


	//N�o registrar pois main menu ja est� registrada.
	//RegisterWindow(gui->desktop);

    kgwmSetFocus (hWindow);
}


void gui_create_logo (void)
{ 
    debug_print ("gui_create_logo: Nothing for now\n");
}

void gui_create_controlmenu (void)
{ 
    debug_print ("gui_create_controlmenu: deprecated\n");
}



/*
 *gui_create_infobox:
 * janelinha de informa��es do sistema.
 * avisa o usuario sobre eventos do sistema.
 *como novos dispositivos encontrados.
 * @todo: Cria buffer dedicado.
 */

void gui_create_infobox (void){
 
    if ( (void *) gui == NULL ){
        debug_print ("gui_create_infobox: gui\n");
        return;
    }else{
        gui->infobox = NULL;
    };    
}


/*
 * Cria a janela para message box.
 */

void gui_create_messagebox (void){
 
    if ( (void *) gui == NULL ){
        debug_print ("gui_create_messagebox: gui\n");
        return;
    }else{
        gui->messagebox = NULL;
    };
}


/*
 ******************
 * cria uma janela para debug do kernel. ser� um progama em kernel mode.
 * @todo: Cria buffer dedicado.
 */

void gui_create_debug (void){
 
    if ( (void *) gui == NULL ){
        debug_print ("gui_create_debug: gui\n");
        return;
    }else{
        gui->debug = NULL;
    };

	// #todo: 
	// Efetuar� ou n�o algum tipo de debug de inicializa��o ou
	// dos elementos b�sicos do sistema. Essa tela de debug
	// pode se mostrar uma op��o importante para o desenvolvedor.
	// Mostraria um log de acertos e problemas durante o processo 
	// de inicializa��o do sistema operacional.
}


void gui_create_navigationbar (void)
{
    debug_print ("gui_create_navigationbar: Nothing for now\n");
}


/*
 **************************
 * gui_create_grid:
 *
 *     Apenas autoriza a utiliza��o de um grid na area de trabalho. 
 *     Havendo essa altoriza��o, o gerenciador de arquivos poder�
 *     desenh�-lo. 
 *     cada item ser� um arquivo da pasta "area de trabalho".
 * 
 * #todo: 
 * Cria buffer dedicado.
 */

void gui_create_grid (void){
 
    if ( (void *) gui == NULL ){
        return;
    }else{
		//initScreenGrid();
        gui->grid = NULL;
    };
}


//screen
//m�trica do dispositivo.

void *guiGetScreenWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->screen;
}



//developer screen

void *guiGetDeveloperScreenWindow (void){

    if ( (void *) gui == NULL )
    {
        //message
        return NULL;
    }


    return (void *) gui->DEVELOPERSCREEN;
}



//background

void *guiGetBackgroundWindow (void){

    if ( (void *) gui == NULL )
    {
        //message
        return NULL;
    }


    return (void *) gui->background;
}


//logo

void *guiGetLogoWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->logo;
}


//desktop window
void *guiGetDesktopWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->desktop;
}


//task bar, top window, mac style

void *guiGetTaskbarWindow (void){

    if ( (void *) gui == NULL )
    {
         return NULL;
    }


    return (void *) gui->taskbar;
}


//main window
//GetDesktopWindow

void *guiGetMainWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->main;
}


//status bar window (do kernel base)

void *guiGetStatusbarWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->statusbar;
}


//grid window (do kernel base)

void *guiGetGridWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->grid;
}


//janela do 'control menu' atual.

void *guiGetMenuWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->menu;
}


//infobox (o sistema envia mensagens de informa��o)

void *guiGetInfoboxWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }


    return (void *) gui->infobox;
}


//tooltip  Janelinha que aparece quando repousa o mouse sobre elemento gr�fico.

void *guiGetTooltipWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->tooltip;
}


//message box do kernel base. 

void *guiGetMessageboxWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->messagebox;
}


//dialogbox do kernel base. 

void *guiGetDialogboxWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->dialogbox;
}


//janela de debug usada pelo kernel base.

void *guiGetDebugWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->debug;
}


// ?
// menubar usada pelo kernel base.

void *guiGetMbhWindowWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->mbhWindow;
}


//top bar.
void *guiGetTopbarWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->topbar;
}


//navigation bar, estilo mobile.
//gerenciada pelo kernel base.

void *guiGetNavigationbarWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->navigationbar;
}


// janela do shell do kernel base.
// na� � um processo.

void *guiGetShellWindowWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->shellWindow;
}


//uma janela filha do shell do kernel base.
void *guiGetShellClientWindowWindow (void){

    if ( (void *) gui == NULL )
    {
        return NULL;
    }

    return (void *) gui->shellClientWindow;
}


// Reposiciona e muda o tamanho da gui->main window.
// configura a �rea de trabalho.

void 
guiSetUpMainWindow ( 
    unsigned long x, 
    unsigned long y, 
    unsigned long width, 
    unsigned long height )
{

    unsigned long Left   = (unsigned long) SCREEN_DEFAULT_LEFT;
    unsigned long Top    = (unsigned long) SCREEN_DEFAULT_TOP;
    unsigned long Width  = (unsigned long) screenGetWidth ();
    unsigned long Height = (unsigned long) screenGetHeight ();


	//check limits

    if ( x > Width || y > Height )
    { return; }

    if ( width > Width || height > Height ) 
    { return; }

    if ( (void *) gui == NULL )
    { return; }


    if ( (void *) gui->main == NULL ){
        // msg?
        return;

    }else{
        replace_window (gui->main, x, y);
        resize_window ( gui->main, width, height);
    };

	//Nothing.
}


// service 301
// Change active window.
// Switch to the next window.
// It changes the foreground thread.
// The new foreground thread will be the thread
// associated with this new window.
// Valid for overlapped windows.

// control + f12

int kgwm_next(void)
{
    struct window_d  *wActive;  // active window
    struct window_d  *n;        // next window

    struct thread_d *t;


    debug_print("kgwm_next: [FIXME]\n");

    //#debug
    //return -1;
    
    
    // #test
    //===============
    // main window
    // See: logon.c
    if ( (void*) gui == NULL ){
        panic ("kgwm_next: gui\n");
    }
    // Do now refresh now.
    if ( (void*) gui->main != NULL ){
        redraw_window ( gui->main, FALSE );
    }
    //===============

    // #todo
    // Se n�o temos uma janela ativa 
    // ent�o temos que providenciar uma.

    if ( active_window < 0){
        panic ("kgwm_next: [FAIL] active_window\n");
        //return -1;
    }

    wActive = (struct window_d *) windowList[active_window];

    if ( (void*) wActive == NULL ){
        panic ("kgwm_next: wActive\n");
    }
    
    if (wActive->used != TRUE || wActive->magic != 1234 )
    {
        panic ("kgwm_next: wActive validation\n");
    }

    // Somente para esse tipo.
    if ( wActive->type != WT_OVERLAPPED ){
        panic("kgwm_next: wActive Invalid type\n");
    }

    //
    // Get next
    //
    
    n = (struct window_d *) wActive->next;
    
    // Se a pr�xima indicada na navega��o � v�lida. Usaremos ela.
    if ( (void*) n != NULL )
    {
        if (n->used == TRUE && n->magic == 1234 )
        {
            debug_print ("kgwm_next: next gotten\n");
            goto __go;
        }
    }

    // A next � inv�lida,
    // ent�o vamos usar a lista que est� no desktop.

    if ( (void*) CurrentDesktop == NULL ){
        panic ("kgwm_next: CurrentDesktop\n");
    }

    if ( CurrentDesktop->desktopUsed  != TRUE && 
         CurrentDesktop->desktopMagic != 1234 )
    {
        panic ("kgwm_next: CurrentDesktop validation\n");
    }


    //
    //  Check list
    //


    // Procurar a janela ativa dentro da lista.
    // � uma list de janelas que pertencem ao desktop atual.

    int i=0;
    struct window_d *tmp;


    int NewWindowSelected=FALSE;

    // limits
    if (CurrentDesktop->lHead < 0 ||CurrentDesktop->lHead >= 8)
    {
        CurrentDesktop->lHead = 0;
    }

    i = CurrentDesktop->lHead;

    tmp = (struct window_d *) CurrentDesktop->list[i]; 
    
    // circula
    // se der certo ou nao
    CurrentDesktop->lHead++;
    if (CurrentDesktop->lHead >= 8)
    {
        CurrentDesktop->lHead = 0;
    }

    // fail
    if ( (void*) tmp == NULL )
    {
        n = (struct window_d *) wActive;
        debug_print("kgwm_next: [fail] list FAIL. Back to active\n");
        goto __go;
    }        

    // ok
    if ( (void*) tmp != NULL )
    {
        if ( tmp->used != TRUE && tmp->magic != 1234 )
        {
            n = (struct window_d *) wActive;
            debug_print("kgwm_next: [fail] list FAIL. Back to active\n");
            goto __go;
        }

        // New window!
        
        if ( tmp->used == TRUE && tmp->magic == 1234 )
        {
            NewWindowSelected = TRUE;
            n = (struct window_d *) tmp;
            debug_print("kgwm_next: [ok] One from the list\n");
            goto __go;
        }
    }


//
// == Go ===========================
//

__go:

    // Last check.

    if ( (void*) n == NULL ){
        panic("kgwm_next: [__go] invalid n\n");
    }

    if (n->used != TRUE || n->magic != 1234 )
    {
        panic("kgwm_next: [__go] n validation\n");
    }

    // Is it a overlapped window?
    if (n->type != WT_OVERLAPPED){
        panic("kgwm_next: [__go] not WT_OVERLAPPED\n");
    }

    //
    // Thread
    //

    // tid
    // #todo: overflow?

    if (n->tid < 0){
        panic("kgwm_next: [__go] no tid for this window\n");
    }

    // Change the foreground thread.
    
    // Temos que tomar cuidado pra n�o colocarmos
    // como foreground thread, uma thread que est� num estado
    // ruim, como zumbi.
    
    t = (struct thread_d *) threadList[ n->tid ];

    if ( (void*) t == NULL ){
        panic("kgwm_next: [__go] invalid thread\n");
    } 

     if ( t->used != TRUE || t->magic != 1234 )
     {
          panic("kgwm_next: [FAIL] thread validation\n");
     } 

     if ( t->state == READY || 
          t->state == RUNNING )
     {
         // ok
         // A next � v�lida.

         debug_print("kgwm_next: [DONE]\n");
         
         
         // Old active window
         debug_print("kgwm_next:  [DONE] kill focus of old active window\n");
         if ( NewWindowSelected == TRUE )
         {
             wActive->active = FALSE;
             kgwmKillFocus(wActive);
             redraw_window(wActive,TRUE);
         }
         
         // New active window.
         debug_print("kgwm_next:  [DONE] redraw new active window\n");
         active_window = n->id;
         n->active = TRUE;
         kgwmSetFocus(n);  // Focus
         foreground_thread = n->tid;          // Change the foreground window.
         redraw_window(n,TRUE);


         // #debug
         draw_text(n,8,8,COLOR_RED,n->name);
         refresh_screen();

         // Envia uma mensagem para a thread pedindo
         // pra ela atualizar a janela principal.
         // 11216.
         kgws_send_to_tid (
             (int) n->tid,               // tid
             (struct window_d *) n,      // NULL
             (int)               11216,  // Message Code
             (unsigned long)     0,      // MAGIC signature, ascii code
             (unsigned long)     0 );    // MAGIC signature, raw byte
            
         debug_print("kgwm_next:  [DONE] done\n");
         return 0;
     } 
     
     // drop
     // Se a condi��o acima n�o foi atendida
     // ent�o continuaremos com a configua��o antiga.
     
     // panic("kgwm_next: [FAIL] thread state\n");
}


/*
 *********************** 
 * register_wm_process: 
 * 
 *     Register the loadable wm process.
 */

// #todo
// We need to have a loadable window manager first,
// this is not valid for the kgws environmet.

int register_wm_process ( pid_t pid ){

    if ( pid<0 || pid >= PROCESS_COUNT_MAX )
    {
        debug_print("register_wm_process: [FAIL] pid\n");
        return -1;
    }

    // We can't
    // The system already has a wm.

    if ( __gpidWindowManager != 0 ){
        debug_print("register_wm_process: [FAIL] The system already has a wm\n");
        return -1;
    }

    // Register.

    __gpidWindowManager = (pid_t) pid;

    return 0;
}


/*
 *********************************************
 * kgwm_mouse_dialog:
 * 
 *     O system_procedure redireciona para c� as mensagens de mouse.
 *     Lembrando que o aplicativo em ring3 chamou o system_procedure
 *  quando invocou o defered procedure. (defered/default)
 */

unsigned long
kgwm_mouse_dialog ( 
    struct window_d *window,
    int msg,
    unsigned long long1,
    unsigned long long2 )
{

    //#todo: No more mouse support for this window server.
    debug_print("kgwm_mouse_dialog: [DEPRECATED]\n");
    
    // #todo
    // Checar validade da estrutura.

    if ( window->isControl == TRUE ){
        return (unsigned long) kgwm_window_control_dialog ( 
                                   window, msg, long1, long2 ); 
    } 

    return 0;
}



unsigned long
kgwm_window_control_dialog ( 
    struct window_d *window,
    int msg,
    unsigned long long1,
    unsigned long long2 )
{

    //#bugbug
    if ( window->isControl != TRUE ){
        panic ("kgwm_window_control_dialog: isControl ?\n");
    }


    switch (msg){

        // mouse button down
        case 30:
			//qual bot�o do mouse?
			switch (long1)
			{
				//bot�o 1	
				case 1:
				    // Se esse controle � um bot�o.
				    if ( window->isButton == 1 )
				    {
						button_down ( window );

			            //#debug
			            //printf ("kgwm_window_control_dialog: mouse keydown, window name %s \n",
			             //   window->name ); 
			            //refresh_screen();
			            return 0;
					}
				    break;
				    
			};
				
		// mouse button up
		case 31:
		    switch (long1)
		    {
				case 1:
				    // Se esse controle � um bot�o.
				    if ( window->isButton == 1 )
				    {
						button_up ( window );

			        //#debug
			        //printf ("kgwm_window_control_dialog: mouse keyup, window name %s \n",
			            //window->name ); 
			        //refresh_screen();
			        
                    if ( window->isMinimize == 1 )
                    {
                        window->control->ke_window = window; //afeta esse bot�o
                        window->control->ke_msg    = MSG_HIDE;
                        window->control->ke_long1  = 0;
                        window->control->ke_long2  = 0;

                        window->control->ke_newmessageFlag = TRUE;
                    }
                    //if ( window->isRestore == 1 ){} //#todo: Criar esse elemento na struct
                    if ( window->isMaximize == 1 )
                    {
                        window->control->ke_window = window; //afeta esse bot�o
                        window->control->ke_msg    = MSG_MAXIMIZE;
                        window->control->ke_long1  = 0;
                        window->control->ke_long2  = 0;

                        window->control->ke_newmessageFlag = TRUE;
                    }
                    if ( window->isClose == 1 )
                    {
                        window->control->ke_window = window; //afeta esse bot�o
                        window->control->ke_msg    = MSG_CLOSE;
                        window->control->ke_long1  = 0;
                        window->control->ke_long2  = 0;

                        window->control->ke_newmessageFlag = TRUE;
                    }

                    return 0;
                }
                break;
            };
            break;
    };

    return 0;
}


// #todo
// Send a message to the control thread of a given pid.
/*
void __kgwm_pidSendMessage ( pid_t pid, int message );
void __kgwm_pidSendMessage ( pid_t pid, int message )
{}
*/


// Send a message to the Init process.
// Only one parameter.
// Remember, the input process's control thread 
// has a defined input model.

void __kgwm_SendMessageToInitProcess ( int message )
{
    struct thread_d  *t;
    int tid = -1;

    thread_type_t InputModel;


    if (message < 0){
        debug_print("__kgwm_SendMessageToInitProcess: [FAIL] message\n");
        return;
    }

    // #todo
    // Check validations.


    // process pointer
    if ( (void*) InitProcess == NULL ){
        panic ("__kgwm_SendMessageToInitProcess: InitProcess\n");
    }

    if ( InitProcess->used != TRUE || InitProcess->magic != 1234 )
    {
        panic ("__kgwm_SendMessageToInitProcess: InitProcess validation\n");
    }

    // thread pointer
    t = (struct thread_d  *) InitProcess->control;

    if ( (void*) t == NULL ){
        panic ("__kgwm_SendMessageToInitProcess: t\n");
    }

    if ( t->used != TRUE || t->magic != 1234 )
    {
        panic ("__kgwm_SendMessageToInitProcess: t validation\n");
    }

    // Get the input model of this thread.
    InputModel = t->input_model;

    // #todo
    // Check input model.

    // Get tid.

    tid = (int) t->tid;

    // #todo: Check overflow

    if (tid<0){
        panic("__kgwm_SendMessageToInitProcess: [ERROR] tid\n");
    }

    // Calling the init process.
    // keyboard events only

    // IN: tid, window, message code, ascii code, raw byte.

    kgws_send_to_tid (
        (int) tid,
        (struct window_d *) 0,
        (int)               message,
        (unsigned long) 12, (unsigned long) 34 );
}




/*
 ******************************************************************* 
 * __kgwm_ps2kbd_procedure:
 * 
 *       Some combinations with control + F1~F12
 */

// Local function.

// Called by kgws_event_dialog in kgws.c
// Called by si_send_longmessage_to_ws and si_send_message_to_ws
// in siws.c 
// #bugbug: We don't wanna call the window server. Not now.

// #important:
// Isso garante que o usu�rio sempre podera alterar o foco
// entre as janelas do kgws usando o teclado, pois essa rotina
// � independente da thread que est� em foreground.

// #todo
// Talvez a gente possa usar algo semelhando quando o window
// server estiver ativo. Mas possivelmente precisaremos 
// usar outra rotina e n�o essa. Pois lidaremos com uma estrutura
// de janela diferente, que esta localizada em ring3.

// From Windows:
// Because the mouse, like the keyboard, 
// must be shared among all the different threads, the OS 
// must not allow a single thread to monopolize the mouse cursor 
// by altering its shape or confining it to a small area of the screen.

// #todo
// This functions is the moment to check the current input model,
// and take a decision. It will help us to compose the event message.
// It is because each environment uses its own event format.

unsigned long 
__kgwm_ps2kbd_procedure ( 
    struct window_d *window, 
    int msg, 
    unsigned long long1, 
    unsigned long long2 )
{

    // #test
    // Testando uma rotina de pintura que usa escape sequence.
    // Queremos que ela funcione na m�quina real.
    // Vamos testar os ponteiros.


    // #todo
    // We need the structure of the current thread.
    // This way we have the current input model for this thread
    // and we can compose an event for this environment.

    // struct thread_d  *t;


    char buffer[128];
    sprintf (buffer,"My super \x1b[8C string!!\n"); 

    int Status = -1;


    if (msg<0){
        return 0;
    }

    switch (msg){


        case MSG_KEYDOWN:
            switch (long1){
            case VK_TAB: printf("TAB\n"); refresh_screen(); break;
            };
            break;

        // Pressionadas: teclas de fun�ao
        case MSG_SYSKEYDOWN: 
            switch (long1){

                case VK_F1:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(0);
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f1\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(0);
                    }
                    return 0;
                    break;

                case VK_F2:
                    if (ctrl_status == TRUE){
                         powertrio_select_client(1);
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f2\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(1);
                    }
                    return 0;
                    break;

                case VK_F3:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(2);
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f3\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(2);
                    }
                    return 0;
                    break;

                case VK_F4:
                    if (ctrl_status == TRUE){
                        powertrio_next();
                    }
                    if (alt_status == 1){
                        printf ("__kgwm_ps2kbd_procedure: alt + f4\n");
                        refresh_screen();
                    }
                    if (shift_status == 1){
                        jobcontrol_switch_console(3);
                    }
                    return 0;
                    break;


                // Reboot
                case VK_F5:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(0);
                        //reboot();
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f5\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                // Send a message to the Init process.
                // 9216 - Launch the redpill application
                case VK_F6:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(1);
                        // #todo: 
                        // shutdown. Only the ring3 applications
                        // can shutdown via qemu for now. 
                        //__kgwm_SendMessageToInitProcess(9216); 
                        return 0; 
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f6\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                // Test 1.
                case VK_F7:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(2);
                       // Send message to init process to launch gdeshell.
                        //__kgwm_SendMessageToInitProcess(9217);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f7\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                // Test 2.
                case VK_F8:
                    if (ctrl_status == TRUE){
                        powertrio_next();
                        // Send message to init process to launch the launcher.
                        //__kgwm_SendMessageToInitProcess(9216); 
                        //__kgwm_SendMessageToInitProcess(9218);  // launch sysmon
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f8\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        kgwm_next();
                    }
                    return 0;
                    break;

                case VK_F9:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(0);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f9\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        __kgwm_SendMessageToInitProcess(9216);  //reboot
                        //reboot();
                    }
                    return 0;
                    break;

                case VK_F10:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(1);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f10\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        __kgwm_SendMessageToInitProcess(9217);  //gdeshell
                    }
                    return 0;
                    break;

                case VK_F11:
                    if (ctrl_status == TRUE){
                        powertrio_select_client(2);
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f11\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                       // #bugbug: Something is wrong with this routine.
                       //__kgwm_SendMessageToInitProcess(9218);  // redpill application
                    }
                    return 0;
                    break;

                case VK_F12:
                    if (ctrl_status == TRUE){
                        powertrio_next();
                    }
                    if (alt_status == TRUE){
                        printf ("__kgwm_ps2kbd_procedure: alt + f12\n");
                        refresh_screen();
                    }
                    if (shift_status == TRUE){
                        __kgwm_SendMessageToInitProcess(9219);  // sysmon
                    }
                    return 0;
                    break;

                default:
                    // nothing
                    return 0;
            
            }
    };

    refresh_screen();
    return 0;
}




int init_gramado (void)
{
    debug_print ("init_gramado: [?? fixme]\n");
    return 0;
}


//
// End.
//

