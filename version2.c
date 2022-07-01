
// Assumptions
// 1) Intead of normal day/month/year format of time we use date as an index where 1 is starting day of library
// 2) If someone borrows any book from library then he/she will always return it even if it is after due date
// 3) Dates are always entered in sorted order
// 4) I have used AVL trees to implement book data base which is sorted on basis of book ID
// 5) Person who does not retuen book on time is stored in faulter list and is not allowed to issue another book

// Disclaimer
// Please enter all names in lowercase alphabets

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
typedef struct book_DB
{
    int book_ID;
    char Title[20];
    char Author[20];
    char subject[20];
    int number_of_copies_issued;
    int number_of_copies_available;
    int demand;                                     // For Q2
    struct book_DB *left;
    struct book_DB *right;
    int height;
} book_DB;
int height(book_DB *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
typedef struct request_queue
{
    char Name_of_student[20];
    char title_of_book[20];
    struct request_queue *next;
} request_queue;
typedef struct borrower_list
{
    char Name_of_student[20];
    char title_of_book[20];
    struct borrower_list *next;
    int return_date;
} borrower_list;

typedef struct faulter_list
{
    char Name_of_student[20];
    struct faulter_list *next;
} faulter_list;

// Used in problem 4 and 5 
typedef struct Node
{
    int data;
    char name[20];
    char name2[20];
    struct Node *next;
} Node;

book_DB *root = NULL;
faulter_list *fptr = NULL;
borrower_list *brptr = NULL;
request_queue *rptr = NULL;
Node *num_list = NULL;
int count = 0; // Used in problem 3

// Code for AVL tree 
// Right rotate
book_DB *rightRotate(book_DB *y)
{
    book_DB *x = y->left;
    book_DB *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotate
book_DB *leftRotate(book_DB *x)
{
    book_DB *y = x->right;
    book_DB *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(book_DB *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
book_DB *insertNode(book_DB *node, book_DB *nptr)
{
    // Find the correct position to insertNode the node and insertNode it
    if (node == NULL)
        return nptr;

    if (nptr->book_ID < node->book_ID)
        node->left = insertNode(node->left, nptr);
    else if (nptr->book_ID > node->book_ID)
        node->right = insertNode(node->right, nptr);
    else
        return node;

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(height(node->left),
                           height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && nptr->book_ID < node->left->book_ID)
        return rightRotate(node);

    if (balance < -1 && nptr->book_ID > node->right->book_ID)
        return leftRotate(node);

    if (balance > 1 && nptr->book_ID > node->left->book_ID)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && nptr->book_ID < node->right->book_ID)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// Used to initialise library with books 
book_DB *make_library()
{
    int N;
    printf("Enter number of books\n");
    scanf("%d", &N);
    fflush(stdin);
    while (N--)
    {
        book_DB *nptr = (book_DB *)malloc(sizeof(book_DB));
        printf("Enter The ID of book\n");
        scanf("%d", &nptr->book_ID);
        fflush(stdin);
        printf("Enter the Title of Book\n");
        gets(nptr->Title);
        fflush(stdin);
        printf("Enter the name of Author of Book\n");
        gets(nptr->Author);
        fflush(stdin);
        printf("Enter the subject of book\n");
        gets(nptr->subject);
        fflush(stdin);
        nptr->number_of_copies_issued = 0;
        nptr->demand = 0;
        printf("Enter the number of copies avaliable of this book\n");
        scanf("%d", &nptr->number_of_copies_available);
        fflush(stdin);
        nptr->left = NULL;
        nptr->right = NULL;
        nptr->height = 1;
        root = insertNode(root, nptr);
    }
}

// Used to view list of borrowers
void view_borrow_list()
{
    borrower_list *curr = brptr;
    while (curr != NULL)
    {
        printf("Name %s Book Name %s Return Date %d\n", curr->Name_of_student, curr->title_of_book, curr->return_date);
        curr = curr->next;
    }
}

// Used to view people present in request queue
void view_request_list()
{
    request_queue *curr = rptr;
    while (curr != NULL)
    {
        printf("Name %s Book Name %s\n", curr->Name_of_student, curr->title_of_book);
        curr = curr->next;
    }
}

// Used to view list of faulters (person who returns book late and is not allowed to request more books )
void view_faulter_list()
{
    faulter_list *curr = fptr;
    while (curr != NULL)
    {
        printf("Name %s\n", curr->Name_of_student);
        curr = curr->next;
    }
}

// Used to print details of each book
void print_details(book_DB *book)
{
    printf("Book ID = %d\n", book->book_ID);
    printf("Title of Book is %s\n", book->Title);
    printf("Author of Book is %s\n", book->Author);
    printf("Number of Books available is %d\n", book->number_of_copies_available);
    printf("Number of Books issued is %d\n", book->number_of_copies_issued);
    printf("Demand of Book is %d\n", book->demand);
}

// Used to view list of books available in library
void printPreOrder(book_DB *curr)
{
    if (curr != NULL)
    {
        print_details(curr);
        printPreOrder(curr->left);
        printPreOrder(curr->right);
    }
}

// Used to check whether given person is faulter or not
int check_faulter(char s[])
{
    faulter_list *curr = fptr;
    while (curr != NULL)
    {
        if (strcmp(s, curr->Name_of_student) == 0)
            return 1;
        curr = curr->next;
    }
    return 0;
}

// Used to count the number of books borrowed by a person already
int count_books(char s[])
{
    int sum = 0;
    borrower_list *curr = brptr;
    while (curr != NULL)
    {
        if (strcmp(curr->Name_of_student, s) == 0)
            sum++;
        curr = curr->next;
    }
    if (sum >= 3)
        return 1;
    else
        return 0;
}

// Used to add a person in list of faulters
void insert_faulter(char s[])
{
    faulter_list *temp = (faulter_list *)malloc(sizeof(faulter_list));
    strcpy(temp->Name_of_student, s);
    temp->next = NULL;
    faulter_list *curr = fptr;
    if (curr == NULL)
        fptr = temp;
    else
    {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = temp;
    }
}
// Used to add faulters in faulter list
int add_faulters(char s[], int date)
{
    borrower_list *curr = brptr;
    int found = 1;
    while (curr != NULL && found)
    {
        if (strcmp(s, curr->Name_of_student) == 0)
        {
            if (date > curr->return_date)
            {
                insert_faulter(s);
                return 1;
            }
        }
        curr = curr->next;
    }
}
// Used to add enteries in request queue
void *add_request(char name[], char book[])
{
    request_queue *curr = rptr;
    request_queue *temp = (request_queue *)malloc(sizeof(request_queue));
    strcpy(temp->Name_of_student, name);
    strcpy(temp->title_of_book, book);
    temp->next = NULL;
    if (curr == NULL)
        rptr = temp;
    else
    {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = temp;
    }
}
// Used to add a person in list of borrowers
void *add_borrower(int date, char s[], char book[])
{
    borrower_list *temp = (borrower_list *)malloc(sizeof(borrower_list));
    borrower_list *curr = brptr;
    strcpy(temp->Name_of_student, s);
    strcpy(temp->title_of_book, book);
    temp->return_date = date + 15;
    temp->next = NULL;
    printf("Hey %s\n", s);
    printf("Here is your requested book\n");
    printf("Kindly return it before day %d\n", temp->return_date);
    printf("If u want to request more books kindly join the queue again\n");
    if (curr == NULL)
        brptr = temp;
    else
    {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = temp;
    }
}
// Used to check whether book is available or not
int book_available(int id, char book[])
{
    book_DB *curr = root;
    while (curr != NULL)
    {
        if ((id == curr->book_ID) && (strcmp(curr->Title, book) == 0))
        {
            if (curr->number_of_copies_available > 0)
            {
                curr->number_of_copies_available--;
                curr->number_of_copies_issued++;
                return 1;
            }
            return 0;
        }
        else if (id > curr->book_ID)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return 0;
}
// Used to process requests
void request_book()
{
    char s[20], book[20];
    int id;
    int date;
    fflush(stdin);
    printf("Enter your Name\n");
    gets(s);
    fflush(stdin);
    printf("Enter the ID of book u want to borrow\n");
    scanf("%d", &id);
    fflush(stdin);
    printf("Enter name of book\n");
    gets(book);
    fflush(stdin);
    printf("Enter todays,s date\n");
    scanf("%d", &date);
    fflush(stdin);
    if (check_faulter(s))
        printf("You can,t request a book beacuse u are in deaulter list\n");
    else if (add_faulters(s, date))
        printf("You can,t request a book beacuse u are in deaulter list\n");
    else if (count_books(s))
        printf("You can,t request beacuse u already have borrowed 3 books\n");
    else if (book_available(id, book))
    {
        if (date < 4)
            count++;
        add_borrower(date, s, book);
    }
    else
    {
        printf("Book Not Available right now , u will soon receive an update once it is avilable\n");
        add_request(s, book);
    }
}
// Used to give book to a person whose request is pending
void give_book(char book[], int date)
{
    request_queue *curr = rptr;
    request_queue *prev = NULL;
    while (curr != NULL)
    {
        if (strcmp(curr->title_of_book, book) == 0)
        {
            add_borrower(date, curr->Name_of_student, book);
            if (curr == rptr)
            {
                rptr = curr->next;
                free(curr);
            }
            else
            {
                prev->next = curr->next;
                free(curr);
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
// Used to return book
void return_book()
{
    char name[20];
    char book[20];
    int date;
    int id;
    fflush(stdin);
    printf("Enter your name\n");
    gets(name);
    fflush(stdin);
    printf("Enter the name of book u are returning\n");
    gets(book);
    fflush(stdin);
    printf("Enter the id of book u want to return\n");
    scanf("%d", &id);
    fflush(stdin);
    printf("Enter today,s date\n");
    scanf("%d", &date);
    fflush(stdin);
    borrower_list *curr = brptr;
    borrower_list *prev = NULL;
    while (curr != NULL)
    {
        if (strcmp(name, curr->Name_of_student) == 0 & strcmp(book, curr->title_of_book) == 0)
        {
            if (check_faulter(name) == 0 && date > curr->return_date)
            {
                printf("You have returned book after due date\n");
                insert_faulter(name);
            }
            if (curr == brptr)
            {
                brptr = curr->next;
                free(curr);
            }
            else
            {
                prev->next = curr->next;
                free(curr);
            }
            book_DB *brr = root;
            printf("Thanks %s for returning the book\n", name);
            while (brr != NULL)
            {
                if (strcmp(book, brr->Title) == 0 && brr->book_ID == id)
                {
                    brr->number_of_copies_available++;
                    book_available(id, book);
                    give_book(book, date);
                    return;
                }
                else if (id > brr->book_ID)
                    brr = brr->right;
                else
                    brr = brr->left;
            }
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("No such entry found\n");
}
int found;      // Vaiable used in Q1
// Used in Q1 for doing traversal of tree
void inorder_Q3(book_DB *curr, char book[])
{
    if (curr != NULL)
    {
        if (strcmp(curr->Title, book) == 0)
            found = 1;
        inorder_Q3(curr->left, book);
        inorder_Q3(curr->right, book);
    }
}
// Used to check whether book with given name exists in lirary or not
int book_check(char book[])
{
    found = 0;
    inorder_Q3(root, book);
    return found;
}
 //Q1 Used to view list of people who will receive book
void view_receive_list()
{
    printf("List of people who will receive books\n");
    request_queue *curr = rptr;
    while (curr != NULL)
    {
        if (book_check(curr->title_of_book))
        {
            printf("%s will receive %s\n", curr->Name_of_student, curr->title_of_book);
        }
        curr = curr->next;
    }
}
// Q2) Used to view books in most demand
void set_demand(book_DB *curr)
{
    if(curr !=  NULL)
    {
        set_demand(curr->left);
        curr->demand = curr->number_of_copies_issued;
        set_demand(curr->right);
    }
}
int max_demand;
// Used ro initialise demand of book
void set_max_demand(book_DB *curr)
{
    if(curr != NULL)
    {
        set_max_demand(curr->left);
        request_queue *rr = rptr;
        while(rr != NULL)
        {
            if(strcmp(rr->title_of_book,curr->Title) == 0)
            curr->demand++;
            rr = rr->next;
        }
        max_demand = max(max_demand,curr->demand);
        set_max_demand(curr->right);
    }
}
void inorder_book(book_DB *curr,int MAX)
{
    if(curr != NULL)
    {
        inorder_book(curr->left,MAX);
        if(curr->demand == MAX)
        printf("%s\n",curr->Title);
        inorder_book(curr->right,MAX);
    }
}
void view_demand()
{
    max_demand = -1;
    set_demand(root);
    set_max_demand(root);
    printf("List of books having maximum demand is\n");
    inorder_book(root,max_demand);
}

// Used in Q4 and Q5
Node *insert_num_list(int x,char name2[],Node *head)
{
    Node *nrr = head;
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = x;
    strcpy(temp->name,name2);
    temp->next = NULL;
    if(nrr == NULL)
    {
        head = temp;
        return head;
    }
    while(nrr->next != NULL)
    nrr = nrr->next;
    nrr->next = temp;
    return head;
}
// Merge function used in merge sort for Q4 and Q5
Node *merge2_LL(Node *head1,Node *head2)
{
    if(head1 == NULL)
    return head2;
    if(head2 == NULL)
    return head1;
    Node *head = NULL;
    Node *curr2 = head2;
    Node *curr1 = head1;
    Node *curr = head;
    while(curr1 != NULL && curr2 != NULL)
    {
        if(curr1->data < curr2->data)
        {
            if(head == NULL)
            {
                head = head1;
                curr = head;
            }
            else
            {
                curr->next = curr1;
                curr = curr1;
            }
            curr1 = curr1->next;
        }
        else
        {
            if(head == NULL)
            {
                head = head2;
                curr = head;
            }
            else
            {
                curr->next = curr2;
                curr = curr2;
            }
            curr2 = curr2->next;
        }
    }
    if(curr1 == NULL)
    curr->next = curr2;
    else if(curr2 == NULL)
    curr->next = curr1;
    return head;
}
// Merge sort used for Q4 and Q5
Node *merge_sort(Node *head)
{
    if(head->next == NULL)
    {
        return head;
    }
    Node *slow = head;
    Node *fast = head;
    while(fast->next != NULL && fast->next->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node *temp = slow->next;
    slow->next = NULL;
    head = merge_sort(head);
    temp = merge_sort(temp);
    head = merge2_LL(head,temp);
    return head;
}
// Reverse Linked List used for reversing linked list in Q4
Node *reverse_LL(Node *head)
{
    Node *curr = head;
    Node *prev = NULL;
    Node *after = NULL;
    while(curr != NULL)
    {
        after = curr->next;
        curr->next = prev;
        prev = curr;
        curr = after;
    }
    return prev;
}
// Function for Q4
void book_issue_sort()
{
    borrower_list *curr = brptr;
    Node *head = NULL;
    while(curr != NULL)
    {
        borrower_list *brr = brptr;
        int countleft = 0,countright = 0;
        while(brr != curr)
        {
            if(strcmp(curr->Name_of_student,brr->Name_of_student) == 0)
            countleft++;
            brr = brr->next;
        }
        while(brr != NULL)
        {
            if(strcmp(curr->Name_of_student,brr->Name_of_student) == 0)
            countright++;
            brr = brr->next;
        }
        if(countright == 1)
        head = insert_num_list(countleft+countright,curr->Name_of_student,head);
        curr = curr->next;
    }
    Node *temp = head;
    head = merge_sort(head);
    head = reverse_LL(head);
    while(temp != NULL)
    {
        printf("%s has borrowed %d books\n ",temp->name,temp->data);
        temp = temp->next;
    }
}
 // Used in Q5
void insert_book(int x,char s1[],char s2[])
{
    Node *nrr = num_list;
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = x;
    strcpy(temp->name,s1);
    strcpy(temp->name2,s2);
    temp->next = NULL;
    if(nrr == NULL)
    {
        num_list = temp;
        return ;
    }
    while(nrr->next != NULL)
    nrr = nrr->next;
    nrr->next = temp;
}
// Function for Q5
void inorder_Q5(book_DB *curr,char s[])
{
    if(curr != NULL)
    {
        inorder_Q5(curr->left,s);
        if(strcmp(s,curr->subject) == 0)
        insert_book(curr->number_of_copies_available,curr->Title,curr->Author);
        inorder_Q5(curr->right,s);
    }
}
void subject_sort(char s[])
{
    inorder_Q5(root,s);
    Node *head = num_list;
    if(head == NULL)
    {
        printf("No books of this subject found\n");
        return ;
    }
    head = merge_sort(head);
    Node *trav = head;
    while(trav != NULL)
    {
        printf("%s wriiten by %s number of copies available %d\n",trav->name,trav->name2,trav->data);
        trav = trav->next;
    }
}
void inorder_Q6(book_DB *curr)
{
    if(curr != NULL)
    {
        inorder_Q6(curr->left);
        if(curr->number_of_copies_available > 0)
        printf("%s\n",curr->Title);
        inorder_Q6(curr->right);
    }
}
void inorder_Q7(book_DB *curr)
{
    if(curr != NULL)
    {
        inorder_Q7(curr->left);
        if(curr->number_of_copies_issued == 0)
        printf("%s\n",curr->Title);
        inorder_Q7(curr->right);
    }
}
void inorder_Q8(book_DB *curr,int from,int to)
{
    if(curr != NULL)
    {
        inorder_Q8(curr->left,from,to);
        if(curr->book_ID >= from && curr->book_ID <= to)
        printf("Book Title %s Book ID %d\n",curr->Title,curr->book_ID);
        inorder_Q8(curr->right,from,to);
    }
}
int main()
{
    // make library function is used to initialise library with some books
    make_library();
    int select = 1;
    while (select)
    {
        printf("Press 0 to Exit\n");
        // Basic Functions to Manage Library
        printf("Press 1 to request a Book\n");
        printf("Press 2 to return a book\n");
        // Function to keep a track of all linked list
        printf("Press 4 to view book list\n");
        printf("Press 5 to view request list\n");
        printf("Press 6 to view borrow list\n");
        printf("Press 7 to view defaulter list\n");
        // DSPD Assignment Question
        printf("Press 8 to view list of students who will receive books\n");
        printf("Press 9 to see the book in most demand recently\n");
        printf("Press 10 to see list of students receiving book in first 3 days\n");
        printf("Press 11 to sort and display the borrower list according to number of books issued\n");
        printf("Press 12 to Sort and display the title and author's name for all books of a particular subject on the basis of number_of_copies_available.\n");
        printf("Press 13 to Display the names of the requested books whose copies are available\n");
        printf("Press 14 to display the titles of all books which have not been issued by anyone\n");
        printf("Press 15 to  Display books in given range\n");
        scanf("%d", &select);
        if (select == 1)
            request_book();
        if (select == 2)
            return_book();
        if (select == 4)
            printPreOrder(root);
        if (select == 5)
            view_request_list();
        if (select == 6)
            view_borrow_list();
        if (select == 7)
            view_faulter_list();
        if (select == 8)
            view_receive_list();
        if(select == 9)
            view_demand();
        if (select == 10)
            printf("Number of students who received books in first 3 days are %d\n", count);
        if(select == 11)
            book_issue_sort();
            if(select == 12)
            {
                char s[20];
                printf("Enter the subject\n");
                fflush(stdin);
                gets(s);
                subject_sort(s);
            }
            if(select == 13)
            {
                printf("List of requested books whose copies are avilable are\n");
                // Since my library directly assigns books which are available to requested people
                // and remove them from request list
                // I am printing the list of books which are available at the current moment
                inorder_Q6(root);
            }
            if(select == 14)
            {
                printf("List of books which are not issued by anyone are\n");
                inorder_Q7(root);
            }
            if(select == 15)
            {
                int from,to;
                printf("Enter from Book ID and to book ID to search in range\n");
                scanf("%d %d",&from,&to);
                printf("List of books haing id in given range are\n");
                inorder_Q8(root,from,to);
            }
    }
    return 0;
}