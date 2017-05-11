#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define BUFFER_SIZE  256

static GtkWidget *new_window;
GtkWidget *create_new_window();
GtkWidget *create_new_window2();
GtkWidget *create_new_window3();
gint CloseAppWindow(GtkWidget *widget,gpointer *data)
{
	gtk_main_quit();
	return(FALSE);
}
void Start_Audit(GtkWidget *widget,gpointer data)
{
	system("service auditd start");
	system("auditctl -e 1");

}
void Stop_Audit(GtkWidget *widget,gpointer data)
{
	system("service auditd stop");
	system("auditctl -e 0");
}
void Encrypt_Audit_Log(GtkWidget *widget,gpointer data)
{
         new_window=create_new_window2();
         gtk_widget_show(new_window);
}
void Check_Server_Audit(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/log.txt");
}
void Check_Database_Audit(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/log.txt");
}
void Check_System_Audit(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/log/audit/audit.log");
}
void Analysis_Server_Audit(GtkWidget *widget,gpointer data)
{
    //system("gksudo gedit /var/summary.txt");
    new_window=create_new_window3();
    gtk_widget_show(new_window);
}
void Analysis_Database_Audit(GtkWidget *widget,gpointer data)
{
  //system("gksudo gedit /var/summary.txt");
    new_window=create_new_window3();
    gtk_widget_show(new_window);
}
void Analysis_System_Audit(GtkWidget *widget,gpointer data)
{
	system("aureport -l >/var/系统登陆分析.txt");
	system("aureport -e >/var/系统事件分析.txt");
	system("aureport -i --failed >/var/失败记录.txt");
	system("aureport -i --success >/var/成功记录.txt");
	system("aureport -i --summary >/var/总报表.txt");
                new_window=create_new_window();
                gtk_widget_show(new_window);
}
void system_log_1(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/系统登陆分析.txt");
}
void system_log_2(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/系统事件分析.txt");
}
void system_log_3(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/失败记录.txt");
}
void system_log_4(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/成功记录.txt");
}
void system_log_5(GtkWidget *widget,gpointer data)
{
	system("gksudo gedit /var/总报表.txt");
}
void Encrypt_button1(GtkWidget *widget,gpointer data)
{
  system("/var/./aes en /var/summary.txt  /var/aes.txt 1234567890123456 1234567890123456");
}
void Encrypt_button2(GtkWidget *widget,gpointer data)
{
  system("/var/./hash -sha1 /var/summary.txt >/var/hash.txt");
}
void Encrypt_button3(GtkWidget *widget,gpointer data)
{
  system("gksudo gedit /var/aes.txt");
}
void Encrypt_button4(GtkWidget *widget,gpointer data)
{
  system("gksudo gedit /var/hash.txt");
}
void Analysis_Server_button1(GtkWidget *widget,gpointer data)
{
    FILE *from_fd,*to_fd;
                int i;
                int count_LOGIN=0,count_LOGOUT=0,count_SYSCALL=0;
                int count_yes=0,count_no=0,count_exit0=0,count_exit1=0;
                char  _buffer[BUFFER_SIZE];
                char *buffer=_buffer;
               char str_type[50];
               char str_name[50];
               char str_id[100];
               char str_success[10];
               char str_exit[10];
               //buffer= (char*)malloc(BUFFER_SIZE * sizeof(char) );
    if((from_fd=fopen("/var/log.txt","r"))==NULL)
    {
        fprintf(stderr, "Open Error1!\n" );
        exit(1);
    }
    if((to_fd=fopen("/var/summary.txt","w+"))==NULL)
    {
        fprintf(stderr, "Open Error2!\n" );
        exit(1);
    }
               while(fgets(buffer,BUFFER_SIZE,from_fd)!=NULL)
               {
                                for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]==':')break;
                                i+=1;
                                buffer+=i;
                                for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]==' ')break;
                                buffer[i]='\0';
                                strcpy(str_type,buffer);
                                if(!strcmp(str_type,"LOGIN")){
                                    count_LOGIN++;
                                //printf("%d\n",count_LOGIN );
                                }
                                else if(!strcmp(str_type,"LOGOUT")){
                                    count_LOGOUT++;
                                //printf("%d\n",count_LOGOUT );
                                }
                                else if(!strcmp(str_type,"SYSCALL")){
                                    count_SYSCALL++;
                                printf("%d\n",count_SYSCALL );
                                }
                                fprintf(stderr," %s\n",str_type );                           
                                i+=2;
                                buffer+=i;
                                /*
                                for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]=='m')break;
                                buffer[i-1]='\0';
                                strcpy(str_time,buffer);
                                printf("%s\n",str_time );
                                i+=1;
                                buffer+=i;
                                */
                                for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]=='=')break;
                                i+=1;
                                buffer+=i;
                                 for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]==' ')break;
                                buffer[i]='\0';
                                strcpy(str_id,buffer);
                                printf(" %s\n",str_id );
                                i+=2;
                                buffer+=i;
                                 for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]=='=')break;
                                i+=1;
                                buffer+=i;
                                 for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]==' ')break;
                                buffer[i]='\0';
                                strcpy(str_success,buffer);
                                if(!strcmp(str_success,"yes")){
                                    count_yes++;
                                //printf("%d\n",count_yes );
                                }
                                else if(!strcmp(str_success,"no")){
                                    count_no++;
                                //printf("%d\n",count_no );
                                }
                                printf(" %s\n",str_success );
                                i+=2;
                                buffer+=i;
                                 for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]=='=')break;
                                i+=1;
                                buffer+=i;
                                 for(i=0;i<BUFFER_SIZE;i++)
                                    if(buffer[i]==' ')break;
                                buffer[i]='\0';
                                strcpy(str_exit,buffer);
                                //if(!strcmp(str_exit,'0')){
                                //    count_exit0++;
                                  //  printf("%d\n",count_exit0 );
                               // }
                                //else  if(!strcmp(str_exit,'1')){
                                //    count_exit1++;
                               // printf("%d\n",count_exit1 );
                               // }
                               //printf(" %s\n",str_exit);
                }
                fprintf(to_fd, "Summary Report\n" );
                fprintf(to_fd, "===========================\n" );
                
                fprintf(to_fd, "Number of logins:%d\n",count_LOGIN );
                fprintf(to_fd,"Number of logouts:%d\n",count_LOGOUT );
                fprintf(to_fd, "Number of syscall:%d\n",count_SYSCALL );
                fprintf(to_fd, "Number of  operation successed:%d\n",count_yes );
                fprintf(to_fd, "Number of  operation failed:%d\n",count_no );
                //fprintf(to_fd, "Number of  exit right:%d\n",count_exit0 );
                //fprintf(to_fd, "Number of  exit wrong:%d\n",count_exit1 );
                
    fclose(from_fd);
    fclose(to_fd);
    from_fd=NULL;
    to_fd=NULL;
    exit(0);
}
void Analysis_Server_button2(GtkWidget *widget,gpointer data)
{
  system("gksudo gedit /var/summary.txt");
}
GtkWidget *create_new_window()
{
	GtkWidget *window;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *button3;
	GtkWidget *main_hbox;
	GtkWidget *main_vbox;
	GtkWidget *label;
	GtkWidget *separator;

	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"系统审计分析");
	// 设置透明度函数
	gtk_window_set_opacity(GTK_WINDOW(window), 0.90);  
	//gtk_widget_set_size_request(window, 500,300);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	//窗口初始位置在屏幕最中央 
               gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
	//connect the delete_event to the main window
	gtk_signal_connect(GTK_OBJECT(window),"Delete_event",
		GTK_SIGNAL_FUNC(CloseAppWindow),NULL);
	main_vbox=gtk_vbox_new(FALSE,0);
                
                label=gtk_label_new("");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
	main_hbox=gtk_hbox_new(TRUE,0);
	button1=gtk_button_new_with_label("系统登陆分析");
	//gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
	gtk_signal_connect(GTK_OBJECT(button1),"clicked",
		GTK_SIGNAL_FUNC(system_log_1),NULL);
	gtk_widget_show(button1);
	button2=gtk_button_new_with_label("系统事件分析");
	//gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
		GTK_SIGNAL_FUNC(system_log_2),NULL);
	gtk_widget_show(button2);
	gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
	gtk_widget_show(main_hbox);
                separator=gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(main_vbox),separator,FALSE,TRUE,5);
                gtk_widget_show(separator);
               
               label=gtk_label_new("");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
              main_hbox=gtk_hbox_new(TRUE,0);
	button1=gtk_button_new_with_label("失败记录");
	//gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
	gtk_signal_connect(GTK_OBJECT(button1),"clicked",
		GTK_SIGNAL_FUNC(system_log_3),NULL);
	gtk_widget_show(button1);
	button2=gtk_button_new_with_label("成功记录");
	//gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
		GTK_SIGNAL_FUNC(system_log_4),NULL);
	gtk_widget_show(button2);
	gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
	gtk_widget_show(main_hbox);
                separator=gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(main_vbox),separator,FALSE,TRUE,5);
                gtk_widget_show(separator);

                label=gtk_label_new("");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
                main_hbox=gtk_hbox_new(TRUE,0);
	button1=gtk_button_new_with_label("总报表");
	//gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
	gtk_signal_connect(GTK_OBJECT(button1),"clicked",
		GTK_SIGNAL_FUNC(system_log_5),NULL);
	gtk_widget_show(button1);
	gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
	gtk_widget_show(main_hbox);
                gtk_container_add (GTK_CONTAINER (window), main_vbox);
	gtk_widget_show(main_vbox);
	gtk_widget_show(window);
	return window;
}
GtkWidget *create_new_window2()
{
  GtkWidget *window;
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *main_hbox;
  GtkWidget *main_vbox;
  GtkWidget *label;
  GtkWidget *separator;

  window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window),"加密审计日志");
  // 设置透明度函数
  gtk_window_set_opacity(GTK_WINDOW(window), 0.90);  
  //gtk_widget_set_size_request(window, 500,300);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  //窗口初始位置在屏幕最中央 
               gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
  //connect the delete_event to the main window
  gtk_signal_connect(GTK_OBJECT(window),"Delete_event",
    GTK_SIGNAL_FUNC(CloseAppWindow),NULL);
  main_vbox=gtk_vbox_new(FALSE,0);
                
                label=gtk_label_new("选择加密算法");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
  main_hbox=gtk_hbox_new(TRUE,0);
  button1=gtk_button_new_with_label("AES");
  //gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
  gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
  gtk_signal_connect(GTK_OBJECT(button1),"clicked",
    GTK_SIGNAL_FUNC(Encrypt_button1),NULL);
  gtk_widget_show(button1);
  button2=gtk_button_new_with_label("HASH");
  //gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
  gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
    GTK_SIGNAL_FUNC(Encrypt_button2),NULL);
  gtk_widget_show(button2);
  gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
  gtk_widget_show(main_hbox);
                separator=gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(main_vbox),separator,FALSE,TRUE,5);
                gtk_widget_show(separator);
               
               label=gtk_label_new("查看加密结果");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
              main_hbox=gtk_hbox_new(TRUE,0);
  button1=gtk_button_new_with_label("AES");
  //gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
  gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
  gtk_signal_connect(GTK_OBJECT(button1),"clicked",
    GTK_SIGNAL_FUNC(Encrypt_button3),NULL);
  gtk_widget_show(button1);
  button2=gtk_button_new_with_label("HASH");
  //gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
  gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
    GTK_SIGNAL_FUNC(Encrypt_button4),NULL);
  gtk_widget_show(button2);
  gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
  gtk_widget_show(main_hbox);
                separator=gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(main_vbox),separator,FALSE,TRUE,5);
                gtk_widget_show(separator);

                main_hbox=gtk_hbox_new(TRUE,0);
  gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
  gtk_widget_show(main_hbox);
                gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_widget_show(main_vbox);
  gtk_widget_show(window);
  return window;
}
GtkWidget *create_new_window3()
{
  GtkWidget *window;
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *main_hbox;
  GtkWidget *main_vbox;
  GtkWidget *label;
  GtkWidget *separator;

  window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window),"服务器日志分析");
  // 设置透明度函数
  gtk_window_set_opacity(GTK_WINDOW(window), 0.90);  
  //gtk_widget_set_size_request(window, 500,300);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  //窗口初始位置在屏幕最中央 
               gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
  //connect the delete_event to the main window
  gtk_signal_connect(GTK_OBJECT(window),"Delete_event",
    GTK_SIGNAL_FUNC(CloseAppWindow),NULL);
  main_vbox=gtk_vbox_new(FALSE,0);
                
                label=gtk_label_new("");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
  main_hbox=gtk_hbox_new(TRUE,0);
  button1=gtk_button_new_with_label("开始分析");
  //gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
  gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
  gtk_signal_connect(GTK_OBJECT(button1),"clicked",
    GTK_SIGNAL_FUNC(Analysis_Server_button1),NULL);
  gtk_widget_show(button1);
  button2=gtk_button_new_with_label("分析结果");
  //gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
  gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
    GTK_SIGNAL_FUNC(Analysis_Server_button2),NULL);
  gtk_widget_show(button2);
  gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
  gtk_widget_show(main_hbox);
                separator=gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(main_vbox),separator,FALSE,TRUE,5);
                gtk_widget_show(separator);
               
    main_hbox=gtk_hbox_new(TRUE,0);
  gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);       
  gtk_widget_show(main_hbox);
                gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_widget_show(main_vbox);
  gtk_widget_show(window);
  return window;
}
int main(int argc, char  *argv[])
{
	GtkWidget *window;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *button3;
	GtkWidget *main_hbox;
	GtkWidget *main_vbox;
	GtkWidget *label;
	GtkWidget *separator;
               
                 gtk_init (&argc, &argv);  
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"auditing system");
	// 设置透明度函数
	gtk_window_set_opacity(GTK_WINDOW(window), 0.90);  
	//gtk_widget_set_size_request(window, 500,300);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	//窗口初始位置在屏幕最中央 
               gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
	//connect the delete_event to the main window
	gtk_signal_connect(GTK_OBJECT(window),"Delete_event",
		GTK_SIGNAL_FUNC(CloseAppWindow),NULL);
	main_vbox=gtk_vbox_new(FALSE,0);
                
                label=gtk_label_new("");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
	main_hbox=gtk_hbox_new(TRUE,0);
	button1=gtk_button_new_with_label("开始审计");
	//gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
	gtk_signal_connect(GTK_OBJECT(button1),"clicked",
		GTK_SIGNAL_FUNC(Start_Audit),NULL);
	gtk_widget_show(button1);
	button2=gtk_button_new_with_label("结束审计");
	//gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
		GTK_SIGNAL_FUNC(Stop_Audit),NULL);
	gtk_widget_show(button2);
	button3=gtk_button_new_with_label("审计加密");
	gtk_box_pack_start(GTK_BOX(main_hbox),button3,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button3),"clicked",
		GTK_SIGNAL_FUNC(Encrypt_Audit_Log),NULL);
	gtk_widget_show(button3);
	gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
	gtk_widget_show(main_hbox);
                separator=gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(main_vbox),separator,FALSE,TRUE,5);
                gtk_widget_show(separator);
               
               label=gtk_label_new("审计查看");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
              main_hbox=gtk_hbox_new(TRUE,0);
	button1=gtk_button_new_with_label("服务器日志");
	//gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
	gtk_signal_connect(GTK_OBJECT(button1),"clicked",
		GTK_SIGNAL_FUNC(Check_Server_Audit),NULL);
	gtk_widget_show(button1);
	button2=gtk_button_new_with_label("数据库日志");
	//gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
		GTK_SIGNAL_FUNC(Check_Database_Audit),NULL);
	gtk_widget_show(button2);
	button3=gtk_button_new_with_label("系统日志");
	gtk_box_pack_start(GTK_BOX(main_hbox),button3,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button3),"clicked",
		GTK_SIGNAL_FUNC(Check_System_Audit),NULL);
	gtk_widget_show(button3);
	gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
	gtk_widget_show(main_hbox);
                separator=gtk_hseparator_new();
                gtk_box_pack_start(GTK_BOX(main_vbox),separator,FALSE,TRUE,5);
                gtk_widget_show(separator);

                label=gtk_label_new("审计分析");
               gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
               gtk_box_pack_start (GTK_BOX (main_vbox), label, FALSE, FALSE, 0);
               gtk_widget_show (label);
                main_hbox=gtk_hbox_new(TRUE,0);
	button1=gtk_button_new_with_label("服务器审计分析");
	//gtk_table_attach(GTK_TABLE(table),button1,1,3,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button1,TRUE,TRUE,10);
	gtk_signal_connect(GTK_OBJECT(button1),"clicked",
		GTK_SIGNAL_FUNC(Analysis_Server_Audit),NULL);
	gtk_widget_show(button1);
	button2=gtk_button_new_with_label("数据库审计分析");
	//gtk_table_attach(GTK_TABLE(table),button2,4,6,1,2,GTK_FILL,GTK_FILL,0,0);
	gtk_box_pack_start(GTK_BOX(main_hbox),button2,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button2),"clicked",
		GTK_SIGNAL_FUNC(Analysis_Database_Audit),NULL);
	gtk_widget_show(button2);
	button3=gtk_button_new_with_label("系统审计分析");
	gtk_box_pack_start(GTK_BOX(main_hbox),button3,TRUE,TRUE,10);
                gtk_signal_connect(GTK_OBJECT(button3),"clicked",
		GTK_SIGNAL_FUNC(Analysis_System_Audit),NULL);
	gtk_widget_show(button3);
	gtk_box_pack_start(GTK_BOX(main_vbox),main_hbox,FALSE,FALSE,0);
	gtk_widget_show(main_hbox);
                gtk_container_add (GTK_CONTAINER (window), main_vbox);
	gtk_widget_show(main_vbox);
	gtk_widget_show(window);
	gtk_main();
	return 0;
}
