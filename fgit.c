#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 10000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 10000
#define MAX_MESSAGE_LENGTH 10000
#define debug(x) printf("%s", x);

int howmanyfile(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    int Number_File_commited = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        // printf("%s\n", entry->d_name);
        Number_File_commited++;
    }
    // printf("numofifle:%d\n", Number_File_commited);

    return Number_File_commited - 2;
}
char current_branch[10000] = "master";
void print_command(int argc, char *const argv[]);
typedef struct time
{
    char year[1000];
    char month[1000];
    char day[1000];
    char hour[1000];
    char minute[1000];
    char second[1000];
} date;

int run_branch(argc, argv);
int run_init(int argc, char *const argv[]);
int create_configs(char *username, char *email);
int config_global(char *username, char *email, int flag);

int run_add(int argc, char *const argv[]);
int add_to_staging(char *filepath);

int run_reset(int argc, char *const argv[]);
int remove_from_staging(char *filepath, int argc, char *const argv[]);

int rootfinder(char *address);
int char_to_int(char *a);
int run_commit(int argc, char *const argv[]);
int inc_last_commit_ID();
bool check_file_directory_exists(char *filepath);
int commit_staged_file(int commit_ID, char *filepath);
int track_file(char *filepath);
bool is_tracked(char *filepath);
int create_commit_file(int commit_ID, char *message);
int find_file_last_commit(char *filepath);
int run_log_author(int argc, char *const argv[]);
int run_log_before(int argc, char *const argv[]);
int run_log_since(int argc, char *const argv[]);
int run_log_branch(int argc, char *const argv[]);
int run_log(int argc, char *const argv[]);

void get_time(char *time_string);
int run_checkout(int argc, char *const argv[]);
int find_file_last_change_before_commit(char *filepath, int commit_ID);
int checkout_file(char *filepath, int commit_ID);

void print_command(int argc, char *const argv[])
{
    for (int i = 1; i < argc; i++)
    {
        fprintf(stdout, "argv[%d]: %s\n", i, argv[i]);
    }
    fprintf(stdout, "\n");
}

typedef struct configdata
{
    char *username;
    char *email;
    char *commitID;
} configdata;

int config_global(char *username, char *email, int flag) // flag=0 : change username // flag=1 change email
{
    FILE *fptr = fopen("C:\\newgitproj\\config.txt", "r");
    char oldemail[1000] = {0};
    char oldusername[10000] = {0};
    fgets(oldusername, 10000, fptr);
    if (oldusername[strlen(oldusername) - 1] == '\n')
        oldusername[strlen(oldusername) - 1] = 0;
    fgets(oldemail, 10000, fptr);
    if (oldemail[strlen(oldemail) - 1] == '\n')
        oldemail[strlen(oldemail) - 1] = 0;
    char newemail[1000] = "email: ";
    char newusername[1000] = "username: ";
    strcat(newemail, email);
    strcat(newusername, username);
    printf("%s\n%s\n", oldemail, oldusername);
    printf("%s\n%s\n", newusername, newemail);
    fclose(fptr);
    fopen("C:\\newgitproj\\config.txt", "w");
    if (flag == 0)
    {
        fputs(newusername, fptr);
        fputc('\n', fptr);
        fputs(oldemail, fptr);
    }
    if (flag == 1)
    {
        fputs(oldusername, fptr);
        fputc('\n', fptr);
        fputs(newemail, fptr);
    }
    fclose(fptr);
}

int config_local(char *username, char *email, int flag) // flag=0 : change username // flag=1 change email
{
    // FILE *fptr = fopen(".\\.fgit\\master\\config.txt", "r");
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char command[10000] = {0};
    strcat(command, rootadd);
    strcat(command, "\\");
    strcat(command, current_branch);
    strcat(command, "\\config.txt");
    FILE *fptr = fopen(command, "r");

    char oldemail[1000] = {0};
    char oldusername[10000] = {0};
    fgets(oldusername, 10000, fptr);
    if (oldusername[strlen(oldusername) - 1] == '\n')
        oldusername[strlen(oldusername) - 1] = 0;
    fgets(oldemail, 10000, fptr);
    if (oldemail[strlen(oldemail) - 1] == '\n')
        oldemail[strlen(oldemail) - 1] = 0;
    char newemail[1000] = "email: ";
    char newusername[1000] = "username: ";

    char last_commit_id[10000] = {0};
    fgets(last_commit_id, 10000, fptr);
    if (last_commit_id[strlen(last_commit_id) - 1] == '\n')
        last_commit_id[strlen(last_commit_id) - 1] = 0;
    char current_commit_id[10000] = {0};
    fgets(current_commit_id, 10000, fptr);
    if (current_commit_id[strlen(current_commit_id) - 1] == '\n')
        current_commit_id[strlen(current_commit_id) - 1] = 0;
    strcat(newemail, email);
    strcat(newusername, username);
    printf("loc:%s\nl:%s\n", oldemail, oldusername);
    printf("loc:%s\nl:%s\n", newusername, newemail);
    fclose(fptr);
    fopen(".\\.fgit\\master\\config.txt", "w");
    if (flag == 0)
    {
        fputs(newusername, fptr);
        fputc('\n', fptr);
        fputs(oldemail, fptr);
        fputc('\n', fptr);
        fputs(last_commit_id, fptr);
        fputc('\n', fptr);
        fputs(current_commit_id, fptr);
    }
    if (flag == 1)
    {
        fputs(oldusername, fptr);
        fputc('\n', fptr);
        fputs(newemail, fptr);
        fputc('\n', fptr);
        fputs(last_commit_id, fptr);
        fputc('\n', fptr);
        fputs(current_commit_id, fptr);
    }
    fclose(fptr);
    return 0;
}

struct configdata readconfig()
{
    char rootadd[10000];
    rootfinder(rootadd);
    char command[10000] = {0};
    strcat(command, rootadd);
    strcat(command, "\\");
    strcat(command, current_branch);
    strcat(command, "\\config.txt");
    FILE *fptr = fopen(command, "r");

    char oldemail[1000] = {0};
    char oldusername[10000] = {0};
    fgets(oldusername, 10000, fptr);
    if (oldusername[strlen(oldusername) - 1] == '\n')
        oldusername[strlen(oldusername) - 1] = 0;
    fgets(oldemail, 10000, fptr);
    if (oldemail[strlen(oldemail) - 1] == '\n')
        oldemail[strlen(oldemail) - 1] = 0;
    char newemail[1000] = "email: ";
    char newusername[1000] = "username: ";

    char last_commit_id[10000] = {0};
    fgets(last_commit_id, 10000, fptr);
    if (last_commit_id[strlen(last_commit_id) - 1] == '\n')
        last_commit_id[strlen(last_commit_id) - 1] = 0;
    char current_commit_id[10000] = {0};
    fgets(current_commit_id, 10000, fptr);
    if (current_commit_id[strlen(current_commit_id) - 1] == '\n')
        current_commit_id[strlen(current_commit_id) - 1] = 0;
    fclose(fptr);
    configdata data;
    data.commitID = last_commit_id;
    data.email = oldemail;
    data.username = oldusername;
    return data;
}

int config_alias(int argc, char *const argv[])
{
}

int rootfinder(char *address)
{
    getcwd(address, 10000);
    char temp[10000] = {0};
    getcwd(temp, 10000);
    int flag = 0;
    while (1)
    {
        if (strcmp(address, "C:\\") == 0)
        {
            chdir(temp);
            *address = '\0';
            return 1;
        }
        if (flag != 0)
        {
            chdir("..");
        }
        getcwd(address, 10000);
        DIR *cur = opendir(address);
        struct dirent *dirs = readdir(cur);
        while ((dirs = readdir(cur)) != NULL)
        {
            if (strcmp(".fgit", dirs->d_name) == 0)
            {
                chdir(temp);
                strcpy(address + strlen(address), "\\.fgit");
                return 0;
            }
        }
        flag++;
    }
}

int compare_time(date one, date two)
{ // 1 = one > two |||  -1 == one<two
    if (char_to_int(one.year) > char_to_int(two.year))
        return 1;
    else if (char_to_int(one.month) > char_to_int(two.month))
        return 1;
    else if (char_to_int(one.day) > char_to_int(two.day))
        return 1;
    else if (char_to_int(one.hour) > char_to_int(two.hour))
        return 1;
    else if (char_to_int(one.minute) > char_to_int(two.minute))
        return 1;
    else if (char_to_int(one.second) > char_to_int(two.second))
        return 1;
    else if (char_to_int(one.second) == char_to_int(two.month))
        return 0;
    return -1;
}

int run_add(int argc, char *const argv[])
{
    if (argc < 3)
    {
        perror("please specify a file");
        return 1;
    }
    // checking exists file or dir?
    bool exists = false;
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        perror("Unable to opening current directory\n");
        return 1;
    }
    if (strcmp(argv[2], "-f") == 0)
    {
        // printf("sd\n");
        for (int i = 3; i < argc; i++)
        {
            exists = false;
            struct dirent *entry;
            rewinddir(dir);
            while ((entry = readdir(dir)) != NULL)
            {
                printf("entry: %s , i:%d\n", entry->d_name, i);
                // printf("arg:%s\n", argv[i]);
                if (strcmp(entry->d_name, argv[i]) == 0)
                    exists = true;
            }
            printf("%d>%d\n", i, exists);
            if (!exists)
            {

                printf("The file or directory %s doesn't exist!!!!!!!!!!!\n", argv[i]);
            }
            else if (exists)
            {
                add_to_staging(argv[i]);
            }
        }
    }
    else
    {

        // TODO: handle command in non-root directories

        // if (!exists)
        // {
        //     printf("The file or directory doesn't exist!");
        //     return 1;
        // }

        int i = 2;
        for (i = 2; i < argc; i++)
        {
            exists = false;
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL)
            {
                printf("arg:%s\n", argv[i]);
                if (strcmp(entry->d_name, argv[i]) == 0)
                    exists = true;
            }
            rewinddir(dir);
            if (exists)
                add_to_staging(argv[i]);
            if (!exists)
            {
                printf("The file or directory %s doesn't exist!\n", argv[i]);
            }
            // printf(" i:%d\n", i);
        }
    }
    return 0;
}

int add_to_staging(char *filepath)
{
    char rootadd[10000] = {0};
    if (rootfinder(rootadd) != 0)
    {
        printf("couldn't find root address\n");
        return 1;
    }
    strcat(rootadd, "\\");
    strcat(rootadd, current_branch);
    strcat(rootadd, "\\staging");
    printf("rootadd:%s\n", rootadd);
    DIR *dir = opendir(rootadd);
    if (dir == NULL)
    {
        printf("Unable to open -> %s", rootadd);
        return 1;
    }
    char stagingadd[10000] = {0};
    strcpy(stagingadd, rootadd);
    strcat(stagingadd, "\\");
    strcat(stagingadd, filepath);
    // printf("stgadd:%s\n", stagingadd);
    if (access(stagingadd, F_OK) != -1)
    {
        // printf("p:%s\n", filepath);
        printf("The file or directory %s has already added\n", filepath);
        return 1;
    }
    else
    {
        char datatype;
        struct stat path_stat;

        if (stat(filepath, &path_stat) == 0)
        {
            if (S_ISDIR(path_stat.st_mode))
            {
                datatype = 'd';
            }
            else
            {
                datatype = 'f';
            }
        }
        else
        {
            printf("worng filepath\n");
        }
        if (datatype == 'f')
        {
            char command[10000] = {0};
            strcat(command, "copy ");
            strcat(command, filepath);
            strcat(command, " .fgit\\");
            strcat(command, current_branch);
            strcat(command, "\\staging");
            // printf("cmd:%s\n", command);
            // char cwd[10000]={0};
            // getcwd(cwd, 10000);
            // printf("cwd:%s\n", cwd);
            system(command);
        }

        else if (datatype == 'd')
        {
            char *command2 = (char *)calloc(10000, sizeof(char));
            // goal : command = "xcopy .\filepath .\.fgit\branch\staging"
            strcat(command2, "Xcopy .\\");
            strcat(command2, filepath);
            strcat(command2, " .fgit\\");
            strcat(command2, current_branch);
            strcat(command2, "\\staging");
            // printf("cmd2:%s\n", command2);
            system(command2);
        }
    }

    //     file = fopen(rootadd, "a");
    // if (file == NULL)
    // {
    //     printf("Unable to add -> .neogit/staging");
    //     return 1;
    // }

    // fprintf(file, "%s\n", filepath);
    // fclose(file);

    return 0;
}

int run_init(int argc, char *const argv[])
{
    char cwd[1024] = {0};
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char tmp_cwd[1024] = {0};
    bool exists = false;
    struct dirent *entry;
    do
    {
        // find .fgit
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error : opening current directory");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".fgit") == 0)
                exists = true;
        }
        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "C:\\") != 0)
        {
            if (chdir("..") != 0)
                return 1;
        }

    } while (strcmp(tmp_cwd, "C:\\") != 0);

    // return to the initial cwd
    if (chdir(cwd) != 0)
        return 1;

    if (!exists)
    {
        if (mkdir(".fgit") != 0)
            return 1;
        printf("repo has  initialized successfully");
        return create_configs("mohamadreza", "salam@gmail.com");
    }
    else
    {
        perror("fgit repository has already initialized");
    }
    return 0;
}

int create_configs(char *username, char *email)
{
    if (mkdir(".fgit\\master") != 0)
        return 1;

    FILE *file = fopen(".fgit\\master\\config.txt", "w");
    if (file == NULL)
        return 1;

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "last_commit_ID: %d\n", 0);
    fprintf(file, "current_commit_ID: %d\n", 0);
    // fprintf(file, "current_branch: %s", "master");

    fclose(file);

    // create commits folder
    if (mkdir(".fgit\\master\\commits") != 0)
        return 1;

    // create files folder
    if (mkdir(".fgit\\master\\files") != 0)
        return 1;

    if (mkdir(".fgit\\master\\staging") != 0)
        return 1;

    if (mkdir(".fgit\\master\\tracks") != 0)
        return 1;

    return 0;
}

int create_configs_newbranch(char *username, char *email , char * branch_name)
{
    char rootadd[1000];
    rootfinder(rootadd);
    char commnad[1000] ={0};
    
    if (mkdir(".fgit\\master") != 0)
        return 1;

    FILE *file = fopen(".fgit\\master\\config.txt", "w");
    if (file == NULL)
        return 1;

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "last_commit_ID: %d\n", 0);
    fprintf(file, "current_commit_ID: %d\n", 0);
    // fprintf(file, "current_branch: %s", "master");

    fclose(file);

    // create commits folder
    if (mkdir(".fgit\\master\\commits") != 0)
        return 1;

    // create files folder
    if (mkdir(".fgit\\master\\files") != 0)
        return 1;

    if (mkdir(".fgit\\master\\staging") != 0)
        return 1;

    if (mkdir(".fgit\\master\\tracks") != 0)
        return 1;

    return 0;
}

int run_reset(int argc, char *const argv[])
{
    if (argc < 3)
    {
        perror("please specify a file");
        return 1;
    }
    // checking there exists file?
    if (strcmp(argv[2], "-f") == 0)
    {
        // printf("ff\n");
        for (int i = 3; i < argc; i++)
        {
            bool exists = false;
            struct dirent *entry;
            char rootadd[10000] = {0};
            rootfinder(rootadd);
            char stagingadd[10000] = {0};
            strcpy(stagingadd, rootadd);
            strcat(stagingadd, "\\");
            strcat(stagingadd, current_branch);
            strcat(stagingadd, "\\staging");
            // strcat(stagingadd, argv[2]);
            printf("stgadd:%s\n", stagingadd);
            DIR *dir = opendir(stagingadd);
            if (dir == NULL)
            {
                perror("Unable to opening current directory");
                return 1;
            }
            rewinddir(dir);
            while ((entry = readdir(dir)) != NULL)
            {
                if (strcmp(entry->d_name, argv[i]) == 0)
                    exists = true;
            }
            closedir(dir);

            if (!exists)
            {
                printf("The file or directory %s doesn't exist!!!!\n", argv[i]);
                continue;
            }
            printf("rftbara:%s\n", argv[i]);
            remove_from_staging(argv[i], argc, argv);
        }
        return 0;
    }
    bool exists = false;
    struct dirent *entry;
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char stagingadd[10000] = {0};
    strcpy(stagingadd, rootadd);
    strcat(stagingadd, "\\");
    strcat(stagingadd, current_branch);
    strcat(stagingadd, "\\staging");
    // strcat(stagingadd, argv[2]);
    printf("stgadd:%s\n", stagingadd);
    DIR *dir = opendir(stagingadd);
    if (dir == NULL)
    {
        perror("Unable to opening current directory");
        return 1;
    }
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, argv[2]) == 0)
            exists = true;
    }
    closedir(dir);

    if (!exists)
    {
        printf("The file or directory %s doesn't exist!!!", argv[2]);
        return 1;
    }

    // TODO: handle command in non-root directories

    remove_from_staging(argv[2], argc, argv);
}

int remove_from_staging(char *filepath, int argc, char *const argv[])
{
    char rootadd[10000] = {0};
    if (rootfinder(rootadd) != 0)
    {
        printf("couldn't find root address");
        return 1;
    }
    strcat(rootadd, "\\");
    strcat(rootadd, current_branch);
    strcat(rootadd, "\\staging");
    printf("stgaddin_remove_stage:%s\n", rootadd);
    DIR *dir = opendir(rootadd);
    // bool exists = false;
    // struct dirent *entry;
    // rewinddir(dir);
    // while (entry = readdir(dir))
    // {
    //     if (strcmp(entry->d_name, argv[2]) == 0)
    //         exists = true;
    // }

    // if (!exists)
    // {
    //     printf("The file or directory %s doesn't exist in(remove stage)!\n" , );
    //     return 1;
    // }

    // if (exists)
    {
        strcat(rootadd, "\\");
        strcat(rootadd, filepath);
        char datatype = 'f';
        struct stat path_stat;
        // rewinddir(path_stat.st_ctime);
        if (stat(filepath, &path_stat) == 0)
        {
            if (S_ISDIR(path_stat.st_mode))
            {
                datatype = 'd';
            }
        }
        printf("datatype:%c\n", datatype);
        if (datatype == 'f')
        {
            char command[10000] = {0};
            strcat(command, "del ");
            printf("rrrrrrrr:%s\n", rootadd);
            strcat(command, rootadd);
            printf("cmd_f:%s\n", command);
            system(command);
        }
        if (datatype == 'd')
        {
            char command[10000] = {0};
            strcat(command, "del ");
            strcat(command, rootadd);
            printf("cmd_d:%s", command);
            system(command);
        }
    }
}

int inc_last_commit_ID() // finally return (lascommit_ID + 1 ) and it will lastcommitid ++ in config.txt
{
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char configadd[10000] = {0};
    strcpy(configadd, rootadd);
    strcat(configadd, "\\");
    strcat(configadd, current_branch);
    char configadd_temp[10000] = {0};
    strcpy(configadd_temp, configadd);
    strcat(configadd, "\\config.txt");
    strcat(configadd_temp, "\\configtmp.txt");
    printf("configadd:%s\n", configadd);
    FILE *file = fopen(configadd, "r");
    if (file == NULL)
    {
        printf("file config.txt cant open\n");
        return -1;
    }

    FILE *tmp_file = fopen(configadd_temp, "w");
    if (tmp_file == NULL)
    {
        printf("file configtmp.txt cant open\n");
        return -1;
    }

    int last_commit_ID;
    char line[MAX_LINE_LENGTH] = {0};
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "last_commit_ID", 14) == 0)
        {
            sscanf(line, "last_commit_ID: %d\n", &last_commit_ID);
            last_commit_ID++;
            fprintf(tmp_file, "last_commit_ID: %d\n", last_commit_ID);
        }
        else
            fprintf(tmp_file, "%s", line);
    }
    fclose(file);
    fclose(tmp_file);

    remove(configadd);
    rename(configadd_temp, configadd);
    printf("lastcommitID:%d\n", last_commit_ID);
    return last_commit_ID;
}

int int_to_char(int a, char *result)
{
    char res[10000] = {0};
    while (a > 0)
    {
        int r = a % 10;
        a /= 10;
        switch (r)
        {
        case 0:
            strcat(res, "0");
            break;
        case 1:
            strcat(res, "1");
            break;
        case 2:
            strcat(res, "2");
            break;
        case 3:
            strcat(res, "3");
            break;
        case 4:
            strcat(res, "4");
            break;
        case 5:
            strcat(res, "5");
            break;
        case 6:
            strcat(res, "6");
            break;
        case 7:
            strcat(res, "7");
            break;
        case 8:
            strcat(res, "8");
            break;
        case 9:
            strcat(res, "9");
            break;

        default:
            printf("invalid iput for number");
            break;
        }
    }
    for (int i = 0; i < strlen(res); i++)
    {
        result[i] = res[strlen(res) - 1 - i];
    }
    // printf("%s", result);
    return 0;
}

int char_to_int(char *a)

{
    int result = 0;
    int j = 1;
    int n = strlen(a);
    for (int i = 0; i < n; i++)
    {
        result += j * (a[n - 1 - i] - '0');
        j *= 10;
    }
    return result;
}

int isDirectoryEmpty(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return -1;
    }

    struct dirent *entry;
    int count = 0;
    rewinddir(dir);
    printf("path:%s\n", path);
    while ((entry = readdir(dir)) != NULL)
    {
        if (++count > 2)
        {
            closedir(dir);
            return 0; // Directory is not empty
        }
    }

    closedir(dir);
    return 1; // Directory is empty
}

int run_commit(int argc, char *const argv[])
{
    if (argc != 4)
    {
        printf("please insert valid commnad for commit\n");
        return -1;
    }
    if (strlen(argv[3]) > 72)
    {
        printf("too long message!\n");
        return -1;
    }
    // checking stage isempty?
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char stagingadd[10000] = {0};
    strcpy(stagingadd, rootadd);
    strcat(stagingadd, "\\");
    strcat(stagingadd, current_branch);
    strcat(stagingadd, "\\staging");
    int a = isDirectoryEmpty(stagingadd);
    if (a == 1)
    {
        printf("The staging directory is empty!\n");
        return -1;
    }
    int Number_File_commited = 0;
    char Number_File_commited_char[10000];
    // how many file would commited?:
    DIR *dir = opendir(stagingadd);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        Number_File_commited++;
    }
    rewinddir(dir);
    // in tabe "." , ".." ro ham mishmore va 2 ta ziad mide.
    int_to_char(Number_File_commited - 2, Number_File_commited_char);
    // mkdir for commit in name current commit id
    rootfinder(rootadd);
    int commit_ID = inc_last_commit_ID(); // to in tabe khodesh lascommit id ziad mikone
    char commitadd[10000] = {0};
    strcat(commitadd, rootadd);
    strcat(commitadd, "\\");
    strcat(commitadd, current_branch);
    strcat(commitadd, "\\commits");
    dir = opendir(commitadd);
    rewinddir(dir);
    char commitfilename[10000] = {0};
    char commit_ID_char[10000] = {0};
    int_to_char(commit_ID, commit_ID_char);
    strcat(commitfilename, commit_ID_char);
    char commnadtomkdir[10000] = {0};
    strcat(commnadtomkdir, "mkdir ");
    strcat(commnadtomkdir, rootadd);
    strcat(commnadtomkdir, "\\");
    strcat(commnadtomkdir, current_branch);
    strcat(commnadtomkdir, "\\commits\\");
    strcat(commnadtomkdir, commit_ID_char);
    system(commnadtomkdir);
    printf("cmd-mkdir:%s\n", commnadtomkdir);

    char command_to_move_stagin_in_commit[10000] = {0};
    strcat(command_to_move_stagin_in_commit, "xcopy /E /H /I /C ");
    strcat(command_to_move_stagin_in_commit, stagingadd);
    strcat(command_to_move_stagin_in_commit, " ");
    strcat(command_to_move_stagin_in_commit, commitadd);
    strcat(command_to_move_stagin_in_commit, "\\");
    strcat(command_to_move_stagin_in_commit, commit_ID_char);
    printf("cdm-move:%s\n", command_to_move_stagin_in_commit);
    system(command_to_move_stagin_in_commit);

    // delete staging file content
    printf("stgaddincommit:%s\n", stagingadd);
    char cmd2[10000] = {0};
    strcat(cmd2, "rmdir /S /Q ");
    strcat(cmd2, stagingadd);
    system(cmd2);
    char command2[10000] = {0};
    strcat(command2, "mkdir ");
    strcat(command2, stagingadd);
    system(command2);
    printf("cmdToNewStage%s\n", command2);

    // should update commit in config

    configdata data;
    data = readconfig();
    printf("commit done successfuly with following information:\n id :%d and %s and %s\n message:%s\n", commit_ID, data.username, data.email, argv[3]);
    char current_commitfile_address[10000] = {0};
    strcat(current_commitfile_address, commitadd);
    strcat(current_commitfile_address, "\\");
    strcat(current_commitfile_address, commit_ID_char);
    strcat(current_commitfile_address, "\\commitinfo.txt");
    printf("cur-commit-file-add-info:%s\n", current_commitfile_address);
    FILE *fptr = fopen(current_commitfile_address, "w");
    fputs("message: ", fptr);
    fputs(argv[3], fptr);
    fputc('\n', fptr);
    fputs(data.username, fptr);
    fputc('\n', fptr);
    fputs("commit_ID: ", fptr);
    fputs(commit_ID_char, fptr);
    fputc('\n', fptr);
    fputs("branch: ", fptr);
    fputs(current_branch, fptr);
    fputc('\n', fptr);
    fputs("Number_File_commited: ", fptr);
    fputs(Number_File_commited_char, fptr);
    char time_string[1000];
    get_time(time_string);
    fputc('\n', fptr);
    fputs("time: ", fptr);
    fputs(time_string, fptr);
    fclose(fptr);
}

void get_time(char *time_string)
{
    time_t now;
    struct tm *local_time;

    // get time
    now = time(NULL);
    local_time = localtime(&now);

    // convert time to string
    strftime(time_string, 80, "%Y-%m-%d %H:%M:%S", local_time);

    // print time
    // printf("now time %s\n", time_string);
}

int run_log(int argc, char *const argv[])
{
    if (argc > 2)
    {
        printf("Invalid input\n");
        return -1;
    }
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char commitadd[10000] = {0};
    strcat(commitadd, rootadd);
    strcat(commitadd, "\\");
    strcat(commitadd, current_branch);
    strcat(commitadd, "\\commits");
    int numberofcommits = howmanyfile(commitadd);
    // printf("numofcommo=its:%d", numberofcommits);
    if (argv[2][0] == '-')
    {
        int n = char_to_int(argv[2] + 1);
        printf("n:%d\n", n);
        for (int i = numberofcommits; i > numberofcommits - n; i--)
        {
            char i_char[100];
            int_to_char(i, i_char);
            char path[1000] = {0};
            strcat(path, commitadd);
            strcat(path, "\\");
            strcat(path, i_char);
            strcat(path, "\\commitinfo.txt");
            FILE *file = fopen(path, "r");
            char line[1000] = {0};
            rewind(file);
            printf("info of commit number %d\n", i);
            while (fgets(line, 10000, file) != NULL)
            {
                printf("%s", line);
            }
            printf("\n");
        }
        return 0;
    }
    for (int i = numberofcommits; i > 0; i--) // TODO : start 1 ya 0?
    {
        char i_char[100];
        int_to_char(i, i_char);
        char path[1000] = {0};
        strcat(path, commitadd);
        strcat(path, "\\");
        strcat(path, i_char);
        strcat(path, "\\commitinfo.txt");
        FILE *file = fopen(path, "r");
        char line[1000] = {0};
        rewind(file);
        printf("info of commit numberr %d\n", i);
        while (fgets(line, 10000, file) != NULL)
        {
            printf("%s", line);
        }
        printf("\n");
    }
}

int run_log_before(int argc, char *const argv[])
{
    // printf("start\n");
    char time_string[1000] = {0};
    strcpy(time_string, argv[3]);
    printf("time_Sstring:%s\n", time_string);
    date time_since;
    strncpy(time_since.year, time_string, 4);
    strncpy(time_since.month, time_string + 5, 2);
    strncpy(time_since.day, time_string + 8, 2);
    strncpy(time_since.hour, time_string + 11, 2);
    strncpy(time_since.minute, time_string + 14, 2);
    strncpy(time_since.second, time_string + 17, 2);
    // printf("qqq:%s\n", time_since.second);
    date time_commit;
    printf("argc:%d\n", argc);
    if (argc > 4)
    {
        printf("please give the date such: \"date\" !");
        return -1;
    }
    if (argc < 4)
    {
        printf("Invalid input!");
        return -1;
    }
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char commitadd[10000] = {0};
    strcat(commitadd, rootadd);
    strcat(commitadd, "\\");
    strcat(commitadd, current_branch);
    strcat(commitadd, "\\commits");
    int numberofcommits = howmanyfile(commitadd);
    printf("sdsds:%d\n", numberofcommits);
    for (int i = numberofcommits; i > 0; i--) // TODO : start 1 ya 0?
    {
        char string_time[1000] = {0};
        strcpy(string_time, argv[3]);
        char i_char[100];
        int_to_char(i, i_char);
        char path[1000] = {0};
        strcat(path, commitadd);
        strcat(path, "\\");
        strcat(path, i_char);
        strcat(path, "\\commitinfo.txt");
        FILE *file = fopen(path, "r");
        char line[1000] = {0};
        rewind(file);
        // checking time
        int flag_print = 0;
        while (fgets(line, 10000, file) != NULL)
        {
            int n = strlen(line);
            if (line[n - 1] == '\n')
                line[n - 1] = 0;
            if (strncmp("time: ", line, 6) == 0)
            {
                int n = strlen(line);
                char tt[1000] = {0};
                strcpy(tt, line + 6);
                printf("tt:%s\n", tt);
                strncpy(time_commit.year, tt, 4);
                strncpy(time_commit.month, tt + 5, 2);
                strncpy(time_commit.day, tt + 8, 2);
                strncpy(time_commit.hour, tt + 11, 2);
                strncpy(time_commit.minute, tt + 14, 2);
                strncpy(time_commit.second, tt + 17, 2);
                // sscanf("%s-%s-%s %s-%s-%s", tt, &time_commit.year, &time_commit.month, &time_commit.day, &time_commit.hour, &time_commit.minute, &time_commit.second);
                printf("comparetime:%d\n", compare_time(time_commit, time_since));
                printf("qqq:%s\n", time_commit.month);

                if (compare_time(time_commit, time_since) <= 0)
                {
                    flag_print = 1;
                }
            }
        }
        if (flag_print)
        {
            printf("info of commit numb:%d\n", i);
            char line[1000] = {0};
            rewind(file);
            while (fgets(line, 10000, file) != NULL)
            {
                printf("%s", line);
            }
            printf("\n");
        }
    }
    printf("end\n");
}

int run_log_since(int argc, char *const argv[])
{
    // printf("start\n");
    char time_string[1000] = {0};
    strcpy(time_string, argv[3]);
    printf("time_Sstring:%s\n", time_string);
    date time_since;
    strncpy(time_since.year, time_string, 4);
    strncpy(time_since.month, time_string + 5, 2);
    strncpy(time_since.day, time_string + 8, 2);
    strncpy(time_since.hour, time_string + 11, 2);
    strncpy(time_since.minute, time_string + 14, 2);
    strncpy(time_since.second, time_string + 17, 2);
    // printf("qqq:%s\n", time_since.second);
    date time_commit;
    printf("argc:%d\n", argc);
    if (argc > 4)
    {
        printf("please give the date such: \"date\" !");
        return -1;
    }
    if (argc < 4)
    {
        printf("Invalid input!");
        return -1;
    }
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char commitadd[10000] = {0};
    strcat(commitadd, rootadd);
    strcat(commitadd, "\\");
    strcat(commitadd, current_branch);
    strcat(commitadd, "\\commits");
    int numberofcommits = howmanyfile(commitadd);
    printf("sdsds:%d\n", numberofcommits);
    for (int i = numberofcommits; i > 0; i--) // TODO : start 1 ya 0?
    {
        char string_time[1000] = {0};
        strcpy(string_time, argv[3]);
        char i_char[100];
        int_to_char(i, i_char);
        char path[1000] = {0};
        strcat(path, commitadd);
        strcat(path, "\\");
        strcat(path, i_char);
        strcat(path, "\\commitinfo.txt");
        FILE *file = fopen(path, "r");
        char line[1000] = {0};
        rewind(file);
        // checking time
        int flag_print = 0;
        while (fgets(line, 10000, file) != NULL)
        {
            int n = strlen(line);
            if (line[n - 1] == '\n')
                line[n - 1] = 0;
            if (strncmp("time: ", line, 6) == 0)
            {
                int n = strlen(line);
                char tt[1000] = {0};
                strcpy(tt, line + 6);
                printf("tt:%s\n", tt);
                strncpy(time_commit.year, tt, 4);
                strncpy(time_commit.month, tt + 5, 2);
                strncpy(time_commit.day, tt + 8, 2);
                strncpy(time_commit.hour, tt + 11, 2);
                strncpy(time_commit.minute, tt + 14, 2);
                strncpy(time_commit.second, tt + 17, 2);
                // sscanf("%s-%s-%s %s-%s-%s", tt, &time_commit.year, &time_commit.month, &time_commit.day, &time_commit.hour, &time_commit.minute, &time_commit.second);
                printf("comparetime:%d\n", compare_time(time_commit, time_since));
                printf("qqq:%s\n", time_commit.month);

                if (compare_time(time_commit, time_since) >= 0)
                {
                    flag_print = 1;
                }
            }
        }
        if (flag_print)
        {
            printf("info of commit numb:%d\n", i);
            char line[1000] = {0};
            rewind(file);
            while (fgets(line, 10000, file) != NULL)
            {
                printf("%s", line);
            }
            printf("\n");
        }
    }
    printf("end\n");
}

int run_log_author(int argc, char *const argv[])
{
    if (argc != 4)
    {
        printf("Invalid input!");
    }
    char author_name[1000] = {0};
    strcat(author_name, argv[3]);
    // printf("name:%s\n", author_name);
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char commitadd[10000] = {0};
    strcat(commitadd, rootadd);
    strcat(commitadd, "\\");
    strcat(commitadd, current_branch);
    strcat(commitadd, "\\commits");
    int numberofcommits = howmanyfile(commitadd);

    for (int i = numberofcommits; i > 0; i--) // TODO : start 1 ya 0?
    {
        // checking ith commit author
        char i_char[100];
        int_to_char(i, i_char);
        char path[1000] = {0};
        strcat(path, commitadd);
        strcat(path, "\\");
        strcat(path, i_char);
        strcat(path, "\\commitinfo.txt");
        FILE *file = fopen(path, "r");
        char line[1000] = {0};
        rewind(file);
        int flag_print = 0;
        // printf("info of commit numberrr %d\n", i);
        while (fgets(line, 1000, file) != NULL)
        {
            int n = strlen(line);
            if (line[n - 1] == '\n')
                line[n - 1] = 0;
            if (strncmp("username: ", line, 10) == 0)
            {
                if (strcmp(line + 10, author_name) == 0)
                {
                    flag_print = 1;
                }
            }
        }
        if (flag_print)
        {
            char line[1000] = {0};
            rewind(file);
            printf("information of %dth commit:\n", i);
            while (fgets(line, 10000, file) != NULL)
            {
                printf("%s", line);
            }
            printf("\n");
        }
    }
}

int run_log_branch(int argc, char *const argv[])
{
    if (argc != 4)
    {
        printf("Invalid input!");
    }
    char branch_name[1000] = {0};
    strcpy(branch_name, argv[3]);
    char rootadd[10000] = {0};
    rootfinder(rootadd);
    char commitadd[10000] = {0};
    strcat(commitadd, rootadd);

    // checking the branch exist?
    bool exist = false;
    struct dirent *entry;
    DIR *dir = opendir(rootadd);
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, branch_name) == 0)
            exist = true;
    }
    if (!exist)
    {
        printf("The branch:%s does not exist!\n", branch_name);
        return -1;
    }
    strcat(commitadd, "\\");
    strcat(commitadd, branch_name);
    strcat(commitadd, "\\commits");
    int numberofcommits = howmanyfile(commitadd);
    // printf("numofcommo=its:%d", numberofcommits);

    for (int i = numberofcommits; i > 0; i--) // TODO : start 1 ya 0?
    {
        char i_char[100];
        int_to_char(i, i_char);
        char path[1000] = {0};
        strcat(path, commitadd);
        strcat(path, "\\");
        strcat(path, i_char);
        strcat(path, "\\commitinfo.txt");
        FILE *file = fopen(path, "r");
        char line[1000] = {0};
        rewind(file);
        printf("info of commit numberrrrrr %d\n", i);
        while (fgets(line, 10000, file) != NULL)
        {
            printf("%s", line);
        }
        printf("\n");
    }
}

int run_branch(int argc, char *const argv[])
{
    // checking branch is exist?
    char branch_name[1000] = {0};
    strcpy(argv[2], branch_name);
    char rootadd[1000];
    rootfinder(rootadd);

    bool exist = false;
    DIR *dir = opendir(rootadd);
    struct dirent *entry;
    rewinddir(dir);
    while (readdir(dir) != NULL)
    {
        if (entry->d_name == branch_name && entry->d_type == DT_DIR)
            exist = true;
    }
    if (exist)
    {
        printf("The branch: %s is already exist!!\n", branch_name);
        return -1;
    }
    char command[1000] = {0};
    strcat(command, "mkdir ");
    strcat(command, rootadd);
    strcat(command, "\\");
    strcat(command, branch_name);
    system(command);

    char commitadd[1000] = {0};
    strcat(commitadd, rootadd);
    strcat(commitadd, "\\");
    strcat(commitadd, current_branch);
    strcat(commitadd, "\\commits");
    // to find last commit
    int Number_File_commited = howmanyfile(commitadd);
    if (Number_File_commited == 0)
    {
        printf("There is no commit here!\n");
        return -1;
    }
    char Number_File_commited_char[1000];
    int_to_char(Number_File_commited, Number_File_commited_char);
    strcat(commitadd, "\\");
    strcat(commitadd, Number_File_commited_char);

}

int main(int argc, char *argv[])
{
    print_command(argc, argv);
    printf("runnnn\n");
    if (strcmp(argv[1], "config") == 0)
    {
        if (strcmp(argv[2], "-global") == 0)
        {
            if (strcmp(argv[3], "user.name") == 0)
            {
                config_global(argv[4], argv[4], 0);
                config_local(argv[4], argv[4], 0);
            }
            if (strcmp(argv[3], "user.email") == 0)
            {
                config_global(argv[4], argv[4], 1);
                config_local(argv[4], argv[4], 1);
            }
        }

        else
        {
            if (strcmp(argv[2], "user.name") == 0)
                return config_local(argv[3], argv[3], 0);
            if (strcmp(argv[2], "user.email") == 0)
                return config_local(argv[3], argv[3], 1);
        }
    }

    char rootadd[10000] = {0};
    rootfinder(rootadd);
    printf("where is root : %s\n", rootadd);

    if (strcmp(argv[1], "init") == 0)
    {

        return run_init(argc, argv);
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        return run_add(argc, argv);
    }
    else if (strcmp(argv[1], "reset") == 0)
    {
        return run_reset(argc, argv);
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        return run_commit(argc, argv);
    }
    else if (strcmp(argv[1], "log") == 0)
    {
        if (strcmp(argv[2], "-branch") == 0)
        {
            return run_log_branch(argc, argv);
        }
        else if (strcmp(argv[2], "-author") == 0)
        {
            return run_log_author(argc, argv);
        }
        else if (strcmp(argv[2], "-since") == 0)
        {
            return run_log_since(argc, argv);
        }
        else if (strcmp(argv[2], "-before") == 0)
        {
            return run_log_before(argc, argv);
        }
        return run_log(argc, argv);
    }
    // else if (strcmp(argv[1], "checkout") == 0)
    // {
    //     return run_checkout(argc, argv);
    // }
    else if (strcmp(argv[1], "branch") == 0)
    {
        if (argv[2] != NULL)
        {
            return run_branch(argc, argv);
        }
        else
            return show_branch(argc, argv);
    }
    printf("Invalid command with fgit");
    return 0;
}