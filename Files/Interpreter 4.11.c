#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "LEXICAL.h"
#include "SYNTACTIC.h"
#include "SEMANTIC.h"
#include "SNSINTERPRETER.h"

#define MAX_LENGTH 127

FILE *SymbolTable, *LookUp, *SourceCode;

struct identifiers{
    int isUsed;
    struct tree *nodeIdent;
    struct identifiers *next, *prev;
}*identhead;

struct tree{
    char name[50];
    int childCount;
    int level, lineNum;
    char value[50], type[20];
    struct tree *children[100], *parent, *sibling;
};

struct tree *tempRoot;

struct token{
    int acceptState;
    char tokenName[20];
    struct token *next, *prev;
}*tHead, *tTail, *tCurr;

int state, line;
char lexeme[50], token[50];

char file[MAX_LENGTH];
void getSym();
void insertTokens();
char initCode[10000];
static GtkWidget *window;
static GtkWidget *table;

static GtkWidget *MenuBox;

static GtkWidget *MenuBar;
static GtkWidget *FileMenu;
static GtkWidget *File;
static GtkWidget *New;
static GtkWidget *Open;
static GtkWidget *Save;
static GtkWidget *SaveAs;
static GtkWidget *Quit;
static GtkWidget *CompileMenu;
static GtkWidget *CompileM;
static GtkWidget *Compile;
static GtkWidget *Run;
static GtkWidget *Stop;
static GtkWidget *sep;

static GtkWidget *MenuToolBar;
static GtkToolItem *NewButton;
static GtkToolItem *OpenButton;
static GtkToolItem *SaveAsButton;
static GtkToolItem *SaveButton;
static GtkToolItem *CompileButton;
static GtkToolItem *RunButton;
static GtkToolItem *StopButton;


static GtkWidget *EditorFrame;
static GtkWidget *EditorText;
static GtkWidget *EditorScroll;
static GtkTextBuffer *EditorBuffer;

static GtkWidget *MessageFrame;
static GtkWidget *MessageText;
static GtkWidget *MessageScroll;
static GtkTextBuffer *MessageBuffer;


static GtkWidget *OutputFrame;
static GtkWidget *OutputText;
static GtkWidget *OutputScroll;
static GtkTextBuffer *OutputBuffer;


static GtkWidget *StatusBar;
static GtkTextBuffer *StatusBuffer;

static GtkWidget *CompileImage;
static GtkWidget *PlayImage;
static GtkWidget *StopImage;
static GtkWidget *SaveImage;
static GtkWidget *SaveAsImage;
static GtkWidget *OpenImage;
static GtkWidget *NewImage;

void ResetMessage() {
    gtk_text_buffer_set_text(MessageBuffer, "", -1);
}

void Message(char* message) {
    GtkTextIter iter;

    gtk_text_buffer_get_iter_at_offset(MessageBuffer, &iter, -1);
    gtk_text_buffer_insert(MessageBuffer, &iter, message, -1);
}

void ResetOutputMessage() {
    gtk_text_buffer_set_text(OutputBuffer, "", -1);
}

void OutputMessage(char* message) {
    GtkTextIter iter;

    gtk_text_buffer_get_iter_at_offset(OutputBuffer, &iter, -1);
    gtk_text_buffer_insert(OutputBuffer, &iter, message, -1);
}

void open(){
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new ("Open File", window, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,
                                          GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkTextIter iter;

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

        strcpy(file, filename);

        //open_file(filename);
        printf("%s", file);
        //g_free(filename);
        SourceCode = fopen(file,"r");
        fscanf(SourceCode, "%[^\0]\0", initCode);
        fclose(SourceCode);
        /* Show text widget info in the statusbar */

        gtk_text_buffer_set_text(EditorBuffer, "", -1);
        StatusBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (EditorText));

        gtk_text_buffer_get_iter_at_offset(EditorBuffer, &iter, 0);
        gtk_text_buffer_insert(EditorBuffer, &iter, initCode, -1);
        ResetMessage();
        Message("File Loaded.");

    }

    gtk_widget_destroy (dialog);
}//edit pa


void save(){
    ResetMessage();
    GtkTextIter start;
    GtkTextIter end;

    char* sourceCode;

    gtk_text_buffer_get_start_iter(EditorBuffer, &start);
    gtk_text_buffer_get_end_iter(EditorBuffer, &end);

    sourceCode = gtk_text_buffer_get_text(EditorBuffer, &start, &end, FALSE);

    remove("SourceCode.sns");

    SourceCode = fopen("SourceCode.sns","w+");

    if(!strcmp(sourceCode,"")) {
        Message("Saving Failed: Source Code is Null.");
        return;
    }
    else Message("SUCCESSFULLY SAVED.");

    fputs(sourceCode, SourceCode);
    fclose(SourceCode);
}

void saveAs(){
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new ("Save File", window, GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL,
                                          GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

    /*if (user_edited_a_new_document)
    {
        gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), default_folder_for_saving);
        gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled document");
    }
    else
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_for_existing_document);
    */ // ano 'to

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        //save_to_file (filename);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
} //edit pa

void compile(){
    save();
    gtk_widget_set_sensitive(RunButton, TRUE);
    char em[30];
    errorCount = 0;
    tempRoot = NULL;

    ResetMessage();
    system("cls");
    LEXICAL(file, tHead);
    tempRoot = SYNTACTIC();
    identhead = SEMANTIC(tempRoot);

    if(!errorCount) Message("\nSuccessfully Compile!...");
    else Message("\nCompilation Failed!...");

    sprintf(em, "\nYou have %d ERROR/S.\n", errorCount);
    Message(em);
}

void run(){
    ResetOutputMessage();

    if(!errorCount){
        gtk_text_view_set_editable(GTK_TEXT_VIEW(EditorText), FALSE);
        gtk_widget_set_sensitive(RunButton, FALSE);
        gtk_widget_set_sensitive(CompileButton, FALSE);
        gtk_widget_set_sensitive(StopButton, TRUE);
        SNSINTERPRETER(tempRoot, identhead);
    }
}

void stop(){
    gtk_widget_set_sensitive(CompileButton, TRUE);
    gtk_widget_set_sensitive(StopButton, FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(EditorText), TRUE);
}

//Statusbar Function
static void update_statusbar (GtkTextBuffer *buffer, GtkStatusbar  *statusbar){
    gchar *msg;
    gint row, col;
    gint count;
    GtkTextIter iter;


    gtk_statusbar_pop (statusbar, 0); // clear any previous message, underflow is allowed

    count = gtk_text_buffer_get_char_count (buffer);

    gtk_text_buffer_get_iter_at_mark (buffer,&iter,gtk_text_buffer_get_insert (buffer));

    row = gtk_text_iter_get_line (&iter);
    col = gtk_text_iter_get_line_offset (&iter);

    msg = g_strdup_printf ("Line %d, Column %d, %d Chars in Document",row+1 ,col, count);

    gtk_statusbar_push (statusbar, 0, msg);

    g_free (msg);
}

static void mark_set_callback (GtkTextBuffer *buffer, const GtkTextIter *new_location, GtkTextMark *mark, gpointer data){
    update_statusbar (buffer, GTK_STATUSBAR (data));
}
//end statusbar function

GdkPixbuf *create_pixbuf(const gchar * filename){
    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if(!pixbuf) {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}

GtkWidget* initGUI(int argc, char** argv){
  	gtk_init(&argc, &argv);

  	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_default_size(GTK_WINDOW(window), 800, 550);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_title(GTK_WINDOW(window), "SNS Programming Language");
    gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("image/sns.png"));


  	table = gtk_table_new(8, 8, FALSE);
  	gtk_table_set_row_spacings(GTK_TABLE(table), 10);
    gtk_table_set_col_spacings(GTK_TABLE(table), 10);
    gtk_container_add(GTK_CONTAINER(window), table);

    //Start Menu
  	MenuBox = gtk_vbox_new(FALSE, 0);
    gtk_table_attach_defaults(GTK_TABLE(table), MenuBox, 0, 8, 0, 1);

    //Start Menubar
    GtkAccelGroup *accel_group = NULL;

    MenuBar = gtk_menu_bar_new();
    FileMenu = gtk_menu_new();
    CompileMenu = gtk_menu_new();

    accel_group = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

    File = gtk_menu_item_new_with_mnemonic("_File");
    New =  gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
    Open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
    Save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
    SaveAs = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, NULL);
    sep = gtk_separator_menu_item_new();
    Quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

    CompileM = gtk_menu_item_new_with_mnemonic("_Compile");
    Compile = gtk_image_menu_item_new_from_stock(GTK_STOCK_EXECUTE, NULL);
    Run = gtk_image_menu_item_new_from_stock(GTK_STOCK_MEDIA_PLAY, NULL);
    Stop = gtk_image_menu_item_new_from_stock(GTK_STOCK_STOP, NULL);


    gtk_widget_add_accelerator(Quit, "activate", accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(File), FileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), New);
    gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), Open);
    gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), Save);
    gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), SaveAs);
    gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(FileMenu), Quit);
    gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar), File);

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(CompileM), CompileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(CompileMenu), Compile);
    gtk_menu_shell_append(GTK_MENU_SHELL(CompileMenu), Run);
    gtk_menu_shell_append(GTK_MENU_SHELL(CompileMenu), Stop);

    gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar), CompileM);

    gtk_box_pack_start(GTK_BOX(MenuBox), MenuBar, FALSE, FALSE, 0);

    //End Menubar

    //Start Toolbar
    MenuToolBar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(MenuToolBar), GTK_TOOLBAR_ICONS);

    gtk_box_pack_start(GTK_BOX(MenuBox), MenuToolBar, FALSE, TRUE, 0);



    NewButton = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
    gtk_toolbar_insert(GTK_TOOLBAR(MenuToolBar), NewButton, -1);

    OpenImage = gtk_image_new_from_file("image/open.png");
    OpenButton = gtk_tool_button_new(OpenImage, "open");
    gtk_toolbar_insert(GTK_TOOLBAR(MenuToolBar), OpenButton, -1);

    SaveImage = gtk_image_new_from_file("image/save.png");
    SaveButton = gtk_tool_button_new(SaveImage, "Save");
    gtk_toolbar_insert(GTK_TOOLBAR(MenuToolBar), SaveButton, -1);

    SaveAsButton = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE_AS);
    gtk_toolbar_insert(GTK_TOOLBAR(MenuToolBar), SaveAsButton, -1);

    CompileImage = gtk_image_new_from_file("image/compile.png");
    CompileButton = gtk_tool_button_new(CompileImage, "Compile");
    gtk_toolbar_insert(GTK_TOOLBAR(MenuToolBar), CompileButton, -1);

    PlayImage = gtk_image_new_from_file("image/play.png");
    RunButton = gtk_tool_button_new(PlayImage, "Run");
  	gtk_toolbar_insert(GTK_TOOLBAR(MenuToolBar), RunButton, -1);

    StopImage = gtk_image_new_from_file("image/stop.png");
    StopButton = gtk_tool_button_new(StopImage, "Stop");
  	gtk_toolbar_insert(GTK_TOOLBAR(MenuToolBar), StopButton, -1);
  	//End Toolbar
  	//End Menu

  	//Start Editor
  	EditorFrame = gtk_frame_new("Code Editor");
  	gtk_frame_set_shadow_type(GTK_FRAME(EditorFrame), GTK_SHADOW_ETCHED_IN);
  	gtk_table_attach_defaults(GTK_TABLE(table), EditorFrame, 0, 8, 1, 5);
    gtk_container_set_border_width(GTK_CONTAINER(EditorFrame), 10);

  	EditorBuffer = gtk_text_buffer_new(NULL);

  	EditorText = gtk_text_view_new_with_buffer(EditorBuffer);
  	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(EditorText), GTK_WRAP_WORD);
  	gtk_widget_grab_focus (EditorText);

  	EditorScroll = gtk_scrolled_window_new(NULL, NULL);
  	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(EditorScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  	gtk_container_add(GTK_CONTAINER(EditorScroll), EditorText);
  	gtk_container_set_border_width(GTK_CONTAINER(EditorScroll), 5);
  	gtk_container_add(GTK_CONTAINER(EditorFrame), EditorScroll);
  	//End Editor

  	//Start Message
  	MessageFrame = gtk_frame_new("Message Box");
  	gtk_frame_set_shadow_type(GTK_FRAME(MessageFrame), GTK_SHADOW_ETCHED_IN);
  	gtk_table_attach_defaults(GTK_TABLE(table), MessageFrame, 0, 4, 5, 7);
    gtk_container_set_border_width(GTK_CONTAINER(MessageFrame), 10);

  	MessageBuffer = gtk_text_buffer_new (NULL);

  	MessageText = gtk_text_view_new_with_buffer(MessageBuffer);
  	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(MessageText), GTK_WRAP_WORD);

  	MessageScroll = gtk_scrolled_window_new(NULL, NULL);
  	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(MessageScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(MessageText), FALSE);

  	gtk_container_add(GTK_CONTAINER(MessageScroll), MessageText);
  	gtk_container_set_border_width(GTK_CONTAINER(MessageScroll), 5);
  	gtk_container_add(GTK_CONTAINER(MessageFrame), MessageScroll);
  	//End Message
    //Output
  	OutputFrame = gtk_frame_new("Output Box");
  	gtk_frame_set_shadow_type(GTK_FRAME(OutputFrame), GTK_SHADOW_ETCHED_IN);
  	gtk_table_attach_defaults(GTK_TABLE(table), OutputFrame, 4, 8, 5, 7);
    gtk_container_set_border_width(GTK_CONTAINER(OutputFrame), 10);

  	OutputBuffer = gtk_text_buffer_new (NULL);

  	OutputText = gtk_text_view_new_with_buffer(OutputBuffer);
  	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(OutputText), GTK_WRAP_WORD);

  	OutputScroll = gtk_scrolled_window_new(NULL, NULL);
  	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(OutputScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(OutputText), FALSE);

  	gtk_container_add(GTK_CONTAINER(OutputScroll), OutputText);
  	gtk_container_set_border_width(GTK_CONTAINER(OutputScroll), 5);
  	gtk_container_add(GTK_CONTAINER(OutputFrame), OutputScroll);
    //End output

    //Start StatusBar
    StatusBar = gtk_statusbar_new();
    gtk_table_attach(GTK_TABLE (table), StatusBar, 0, 8, 7, 8, GTK_EXPAND | GTK_FILL,  0, 0, 0);
    //End StatusBar

    return window;
}

void newm(int argc, char** argv){
    //New WIndow
    gtk_widget_destroy(window);
    GtkWidget* window = initGUI(argc, argv);

    //gtk_text_buffer_get_iter_at_offset(EditorBuffer, &iter, 0);
    //gtk_text_buffer_insert(EditorBuffer, &iter, initCode, -1);
    // Show text widget info in the statusbar
    StatusBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (EditorText));

    g_signal_connect_object (StatusBuffer, "changed", G_CALLBACK (update_statusbar), StatusBar, 0);

    g_signal_connect_object (StatusBuffer, "mark_set", G_CALLBACK (mark_set_callback), StatusBar,0);

    update_statusbar (StatusBuffer, GTK_STATUSBAR (StatusBar));
    //End Statusbar

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //Menubar
    g_signal_connect(G_OBJECT(New), "activate", G_CALLBACK(newm), NULL);
    g_signal_connect(G_OBJECT(Open), "activate", G_CALLBACK(open), NULL);
    g_signal_connect(G_OBJECT(Save), "activate", G_CALLBACK(save), NULL);
    g_signal_connect(G_OBJECT(SaveAs), "activate", G_CALLBACK(saveAs), NULL);
    g_signal_connect(G_OBJECT(Quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(Compile), "activate", G_CALLBACK(compile), NULL);
    g_signal_connect(G_OBJECT(Run), "activate", G_CALLBACK(run), NULL);

    //ToolBar
    g_signal_connect(G_OBJECT(NewButton), "clicked", G_CALLBACK(newm), window);
    g_signal_connect(G_OBJECT(OpenButton), "clicked", G_CALLBACK(open), window);
    g_signal_connect(G_OBJECT(SaveButton), "clicked", G_CALLBACK(save), window);
    g_signal_connect(G_OBJECT(SaveAsButton), "clicked", G_CALLBACK(saveAs), window);
    g_signal_connect(G_OBJECT(CompileButton), "clicked", G_CALLBACK(compile), window);
    g_signal_connect(G_OBJECT(RunButton), "clicked", G_CALLBACK(run), window);
    g_signal_connect(G_OBJECT(StopButton), "clicked", G_CALLBACK(stop), window);

    Message("Welcome to SNS!\n");
    gtk_widget_show_all(window);
    gtk_main();
}


int main(int argc, char** argv){
    GtkWidget* window = initGUI(argc, argv);


    tHead = tTail = tCurr = NULL;

    insertTokens();

    //printf("FILE NAME: ");
	//gets(file);
	strcpy(file,"SourceCode.sns");
    gtk_widget_set_sensitive(RunButton, FALSE);
    gtk_widget_set_sensitive(StopButton, FALSE);

    if(!stricmp(file,"SourceCode.sns")){
        GtkTextIter iter;

        SourceCode = fopen("SourceCode.sns","r");
        fscanf(SourceCode, "%[^\0]\0", initCode);
        fclose(SourceCode);

        gtk_text_buffer_get_iter_at_offset(EditorBuffer, &iter, 0);
        gtk_text_buffer_insert(EditorBuffer, &iter, initCode, -1);
        /* Show text widget info in the statusbar */
        StatusBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (EditorText));

        g_signal_connect_object (StatusBuffer, "changed", G_CALLBACK (update_statusbar), StatusBar, 0);

        g_signal_connect_object (StatusBuffer, "mark_set", G_CALLBACK (mark_set_callback), StatusBar,0);

        update_statusbar (StatusBuffer, GTK_STATUSBAR (StatusBar));
        //End Statusbar

        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        //Menubar
        g_signal_connect(G_OBJECT(New), "activate", G_CALLBACK(newm), NULL);
        g_signal_connect(G_OBJECT(Open), "activate", G_CALLBACK(open), NULL);
        g_signal_connect(G_OBJECT(Save), "activate", G_CALLBACK(save), NULL);
        g_signal_connect(G_OBJECT(SaveAs), "activate", G_CALLBACK(saveAs), NULL);
        g_signal_connect(G_OBJECT(Quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(G_OBJECT(Compile), "activate", G_CALLBACK(compile), NULL);
        g_signal_connect(G_OBJECT(Run), "activate", G_CALLBACK(run), NULL);
        //ToolBar
        g_signal_connect(G_OBJECT(NewButton), "clicked", G_CALLBACK(newm), window);
        g_signal_connect(G_OBJECT(OpenButton), "clicked", G_CALLBACK(open), window);
        g_signal_connect(G_OBJECT(SaveButton), "clicked", G_CALLBACK(save), window);
        g_signal_connect(G_OBJECT(SaveAsButton), "clicked", G_CALLBACK(saveAs), window);
        g_signal_connect(G_OBJECT(CompileButton), "clicked", G_CALLBACK(compile), window);
        g_signal_connect(G_OBJECT(RunButton), "clicked", G_CALLBACK(run), window);
        g_signal_connect(G_OBJECT(StopButton), "clicked", G_CALLBACK(stop), window);
        Message("Welcome to SNS!\n");
        Message("File Loaded.");
        gtk_widget_show_all(window);
        gtk_main();
    }
    else printf("\n\tInvalid file name!...\n\tPRESS any key to Continue.\n\n");
}

void insertTokens(){
    LookUp = fopen("LookUpTable.sns","r+");
    int aState;
    char token[20];

    while(fscanf(LookUp,"%s %d",&token,&aState) != EOF){
        tCurr = (struct token*) malloc(sizeof(struct token));
        tCurr -> next = tCurr -> prev = NULL;
        if (tHead == NULL){
            tHead = tTail = tCurr;
            tCurr -> acceptState = aState;
            strcpy(tCurr->tokenName, token);
        }//if
        else{
            tTail -> next = tCurr;
            tCurr -> prev = tTail;
            tTail = tCurr;
            tCurr -> acceptState = aState;
            strcpy(tCurr -> tokenName, token);
        }//else
    }//while

    fclose(LookUp);
    tTail -> next = NULL;
}//insertToken()




