# DSA-Project---Online-Shopping-Cart-System


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTS 30
#define MAX_NAME 80
#define MAX_CATEGORY 30
#define MAX_ORDERS 100
#define MAX_QUEUE 100
#define STACK_SIZE 10

/*
=========================================================
                COSMETIC STORE
                  DSA PROJECT
                IN C LANGUAGE
=========================================================

DSA CONCEPTS USED:

1. Structures
2. Arrays
3. Singly Linked List       -> Shopping Cart
4. Stack                    -> Recently Viewed Products
5. Queue                    -> Order Processing
6. Linear Search            -> Product Search
7. Binary Search            -> Product ID Search
8. Bubble Sort              -> Product Sorting
9. Dynamic Memory Allocation
10. File Handling
11. Pointers
12. Functions
13. String Handling
=========================================================
*/

/* ===================== STRUCTURES ===================== */

typedef struct
{
    int id;
    char name[MAX_NAME];
    char category[MAX_CATEGORY];
    float price;
    float rating;
    int stock;
} Product;

typedef struct CartNode
{
    int productId;
    int quantity;
    struct CartNode *next;
} CartNode;

typedef struct
{
    int orderId;
    float amount;
    char status[30];
} Order;

typedef struct
{
    int items[MAX_QUEUE];
    int front;
    int rear;
} Queue;

typedef struct
{
    int items[STACK_SIZE];
    int top;
} Stack;

/* ===================== BEAUTY PRODUCTS ===================== */

Product products[MAX_PRODUCTS] =
{
    {101, "Gentle Foaming Face Wash", "Skincare", 399.00f, 4.5f, 30},
    {102, "Niacinamide Face Serum", "Skincare", 599.00f, 4.6f, 25},
    {103, "Hyaluronic Acid Serum", "Skincare", 649.00f, 4.5f, 22},
    {104, "Lightweight Gel Moisturizer", "Skincare", 549.00f, 4.4f, 28},
    {105, "SPF 50 Sunscreen", "Sunscreen", 699.00f, 4.7f, 35},
    {106, "Clay Face Mask", "Skincare", 449.00f, 4.3f, 20},
    {107, "Micellar Makeup Remover", "Skincare", 499.00f, 4.5f, 24},
    {108, "Hydrating Sheet Mask", "Skincare", 149.00f, 4.2f, 50},
    {109, "Matte Liquid Foundation", "Makeup", 899.00f, 4.4f, 18},
    {110, "Full Coverage Concealer", "Makeup", 649.00f, 4.5f, 20},
    {111, "Nude Pink Lipstick", "Makeup", 499.00f, 4.6f, 30},
    {112, "Long Stay Liquid Lipstick", "Makeup", 599.00f, 4.5f, 26},
    {113, "Volumizing Mascara", "Makeup", 449.00f, 4.3f, 25},
    {114, "Waterproof Eyeliner", "Makeup", 349.00f, 4.4f, 32},
    {115, "Soft Glow Blush", "Makeup", 549.00f, 4.5f, 21},
    {116, "Highlighter Palette", "Makeup", 799.00f, 4.6f, 15},
    {117, "Everyday Eyeshadow Palette", "Makeup", 999.00f, 4.7f, 14},
    {118, "Hair Repair Shampoo", "Haircare", 499.00f, 4.4f, 30},
    {119, "Hydrating Hair Conditioner", "Haircare", 549.00f, 4.5f, 27},
    {120, "Anti-Frizz Hair Serum", "Haircare", 599.00f, 4.3f, 22},
    {121, "Nourishing Hair Mask", "Haircare", 699.00f, 4.5f, 18},
    {122, "Rose Body Lotion", "Body Care", 399.00f, 4.4f, 25},
    {123, "Moisturizing Body Wash", "Body Care", 449.00f, 4.3f, 28},
    {124, "Exfoliating Body Scrub", "Body Care", 499.00f, 4.5f, 20},
    {125, "Hydrating Lip Balm", "Lip Care", 199.00f, 4.5f, 45},
    {126, "Lip Sleeping Mask", "Lip Care", 349.00f, 4.6f, 25},
    {127, "Floral Eau De Parfum", "Fragrance", 1299.00f, 4.6f, 15},
    {128, "Fresh Body Mist", "Fragrance", 599.00f, 4.3f, 24},
    {129, "Makeup Brush Set", "Beauty Tools", 749.00f, 4.5f, 18},
    {130, "Makeup Sponge Set", "Beauty Tools", 299.00f, 4.4f, 35}
};

int productCount = MAX_PRODUCTS;
CartNode *cartHead = NULL;
Order orders[MAX_ORDERS];
int orderCount = 0;
int nextOrderId = 1001;
Queue orderQueue;
Stack recentStack;

/* ===================== UTILITY FUNCTIONS ===================== */

void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int readInt(const char *prompt)
{
    int value;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1)
        {
            clearInputBuffer();
            return value;
        }
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
    }
}

void pauseScreen(void)
{
    printf("\nPress Enter to continue...");
    getchar();
}

void line(void)
{
    printf("-------------------------------------------------------------------------------\n");
}

void printHeader(void)
{
    printf("\n");
    printf("===============================================================\n");
    printf("                    GLOWCART BEAUTY STORE                     \n");
    printf("===============================================================\n");
}

Product *getProductById(int id)
{
    int i;
    for (i = 0; i < productCount; i++)
    {
        if (products[i].id == id)
            return &products[i];
    }
    return NULL;
}

/* ===================== STACK ===================== */

void initStack(Stack *s)
{
    s->top = -1;
}

void pushRecent(Stack *s, int productId)
{
    int i;

    /* Remove duplicate if product is already in stack. */
    for (i = 0; i <= s->top; i++)
    {
        if (s->items[i] == productId)
        {
            int j;
            for (j = i; j < s->top; j++)
                s->items[j] = s->items[j + 1];
            s->top--;
            break;
        }
    }

    if (s->top < STACK_SIZE - 1)
    {
        s->top++;
        s->items[s->top] = productId;
    }
    else
    {
        for (i = 0; i < STACK_SIZE - 1; i++)
            s->items[i] = s->items[i + 1];

        s->items[STACK_SIZE - 1] = productId;
    }
}

void displayRecent(Stack *s)
{
    int i;

    if (s->top == -1)
    {
        printf("\nNo recently viewed products.\n");
        return;
    }

    printf("\n========== RECENTLY VIEWED PRODUCTS ==========\n");

    for (i = s->top; i >= 0; i--)
    {
        Product *p = getProductById(s->items[i]);

        if (p != NULL)
        {
            printf("%d - %s - Rs. %.2f\n",
                   p->id, p->name, p->price);
        }
    }
}

/* ===================== QUEUE ===================== */

void initQueue(Queue *q)
{
    q->front = 0;
    q->rear = -1;
}

int isQueueEmpty(Queue *q)
{
    return q->front > q->rear;
}

int isQueueFull(Queue *q)
{
    return q->rear == MAX_QUEUE - 1;
}

int enqueue(Queue *q, int orderId)
{
    if (isQueueFull(q))
    {
        printf("Order processing queue is full.\n");
        return 0;
    }

    q->rear++;
    q->items[q->rear] = orderId;
    return 1;
}

int dequeue(Queue *q)
{
    int orderId;

    if (isQueueEmpty(q))
        return -1;

    orderId = q->items[q->front];
    q->front++;

    if (q->front > q->rear)
    {
        q->front = 0;
        q->rear = -1;
    }

    return orderId;
}

void displayQueue(Queue *q)
{
    int i;

    if (isQueueEmpty(q))
    {
        printf("\nOrder processing queue is empty.\n");
        return;
    }

    printf("\n========== ORDER PROCESSING QUEUE ==========\n");

    for (i = q->front; i <= q->rear; i++)
        printf("Order ID: %d\n", q->items[i]);
}

/* ===================== PRODUCT DISPLAY ===================== */

void printProduct(Product *p)
{
    printf("%-5d %-30s %-15s Rs. %-8.2f %-5.1f %-5d\n",
           p->id, p->name, p->category,
           p->price, p->rating, p->stock);
}

void displayProducts(Product arr[], int count)
{
    int i;

    line();

    printf("%-5s %-30s %-15s %-12s %-5s %-5s\n",
           "ID", "PRODUCT", "CATEGORY", "PRICE", "RATE", "STOCK");

    line();

    for (i = 0; i < count; i++)
        printProduct(&arr[i]);

    line();
}

/* ===================== BROWSE PRODUCTS ===================== */

void browseProducts(void)
{
    int choice;
    int i;
    char category[MAX_CATEGORY];

    while (1)
    {
        printf("\n========== BEAUTY PRODUCT CATALOGUE ==========\n");
        printf("1.  All Products\n");
        printf("2.  Skincare\n");
        printf("3.  Sunscreen\n");
        printf("4.  Makeup\n");
        printf("5.  Haircare\n");
        printf("6.  Body Care\n");
        printf("7.  Lip Care\n");
        printf("8.  Fragrance\n");
        printf("9.  Beauty Tools\n");
        printf("10. Back\n");

        choice = readInt("Enter choice: ");

        if (choice == 10)
            return;

        if (choice == 1)
        {
            displayProducts(products, productCount);
            pauseScreen();
            continue;
        }

        switch (choice)
        {
            case 2: strcpy(category, "Skincare"); break;
            case 3: strcpy(category, "Sunscreen"); break;
            case 4: strcpy(category, "Makeup"); break;
            case 5: strcpy(category, "Haircare"); break;
            case 6: strcpy(category, "Body Care"); break;
            case 7: strcpy(category, "Lip Care"); break;
            case 8: strcpy(category, "Fragrance"); break;
            case 9: strcpy(category, "Beauty Tools"); break;
            default:
                printf("Invalid choice.\n");
                continue;
        }

        printf("\n");
        line();

        printf("%-5s %-30s %-15s %-12s %-5s %-5s\n",
               "ID", "PRODUCT", "CATEGORY", "PRICE", "RATE", "STOCK");

        line();

        for (i = 0; i < productCount; i++)
        {
            if (strcmp(products[i].category, category) == 0)
                printProduct(&products[i]);
        }

        line();
        pauseScreen();
    }
}

/* ===================== LINEAR SEARCH ===================== */

void searchProducts(void)
{
    char keyword[80];
    int i;
    int found = 0;

    printf("\n========== SEARCH BEAUTY PRODUCTS ==========\n");
    printf("Enter product name or category: ");

    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    if (strlen(keyword) == 0)
    {
        printf("Search keyword cannot be empty.\n");
        return;
    }

    for (i = 0; keyword[i]; i++)
        keyword[i] = (char)tolower((unsigned char)keyword[i]);

    printf("\nSearch results:\n");
    line();

    for (i = 0; i < productCount; i++)
    {
        char nameLower[MAX_NAME];
        char categoryLower[MAX_CATEGORY];
        int j;

        strcpy(nameLower, products[i].name);
        strcpy(categoryLower, products[i].category);

        for (j = 0; nameLower[j]; j++)
            nameLower[j] = (char)tolower((unsigned char)nameLower[j]);

        for (j = 0; categoryLower[j]; j++)
            categoryLower[j] = (char)tolower((unsigned char)categoryLower[j]);

        if (strstr(nameLower, keyword) != NULL ||
            strstr(categoryLower, keyword) != NULL)
        {
            printProduct(&products[i]);
            found = 1;
        }
    }

    line();

    if (!found)
    {
        printf("No product found.\n");
        return;
    }

    {
        int id = readInt("Enter product ID to view details, or 0 to return: ");

        if (id != 0)
        {
            Product *p = getProductById(id);

            if (p != NULL)
            {
                printf("\n========== PRODUCT DETAILS ==========\n");
                printf("Product  : %s\n", p->name);
                printf("Category : %s\n", p->category);
                printf("Price    : Rs. %.2f\n", p->price);
                printf("Rating   : %.1f/5\n", p->rating);
                printf("Stock    : %d\n", p->stock);
                pushRecent(&recentStack, p->id);
            }
            else
            {
                printf("Invalid product ID.\n");
            }
        }
    }
}

/* ===================== BUBBLE SORT ===================== */

void sortProducts(void)
{
    Product temp[MAX_PRODUCTS];
    int choice;
    int i, j;

    for (i = 0; i < productCount; i++)
        temp[i] = products[i];

    printf("\n========== SORT BEAUTY PRODUCTS ==========\n");
    printf("1. Price: Low to High\n");
    printf("2. Price: High to Low\n");
    printf("3. Rating: High to Low\n");

    choice = readInt("Enter choice: ");

    for (i = 0; i < productCount - 1; i++)
    {
        for (j = 0; j < productCount - i - 1; j++)
        {
            int swap = 0;

            if (choice == 1 && temp[j].price > temp[j + 1].price)
                swap = 1;
            else if (choice == 2 && temp[j].price < temp[j + 1].price)
                swap = 1;
            else if (choice == 3 && temp[j].rating < temp[j + 1].rating)
                swap = 1;

            if (swap)
            {
                Product t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    if (choice >= 1 && choice <= 3)
        displayProducts(temp, productCount);
    else
        printf("Invalid choice.\n");
}

/* ===================== BINARY SEARCH ===================== */

void binarySearchById(void)
{
    Product sorted[MAX_PRODUCTS];
    int i, left, right, mid, id;

    for (i = 0; i < productCount; i++)
        sorted[i] = products[i];

    /* Sort temporary array by Product ID. */
    for (i = 0; i < productCount - 1; i++)
    {
        int j;

        for (j = 0; j < productCount - i - 1; j++)
        {
            if (sorted[j].id > sorted[j + 1].id)
            {
                Product temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    id = readInt("Enter Beauty Product ID for binary search: ");

    left = 0;
    right = productCount - 1;

    while (left <= right)
    {
        mid = left + (right - left) / 2;

        if (sorted[mid].id == id)
        {
            printf("\n========== PRODUCT FOUND ==========\n");
            printProduct(&sorted[mid]);
            pushRecent(&recentStack, sorted[mid].id);
            return;
        }
        else if (sorted[mid].id < id)
            left = mid + 1;
        else
            right = mid - 1;
    }

    printf("Product not found.\n");
}

/* ===================== LINKED LIST CART ===================== */

CartNode *findCartItem(int productId)
{
    CartNode *current = cartHead;

    while (current != NULL)
    {
        if (current->productId == productId)
            return current;

        current = current->next;
    }

    return NULL;
}

int cartItemCount(void)
{
    CartNode *current = cartHead;
    int count = 0;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

/* ===================== ADD TO CART ===================== */

void addToCart(void)
{
    int id;
    int quantity;
    Product *p;
    CartNode *existing;

    id = readInt("Enter Beauty Product ID: ");
    p = getProductById(id);

    if (p == NULL)
    {
        printf("Product not found.\n");
        return;
    }

    if (p->stock <= 0)
    {
        printf("Product is out of stock.\n");
        return;
    }

    quantity = readInt("Enter quantity: ");

    if (quantity <= 0)
    {
        printf("Quantity must be greater than zero.\n");
        return;
    }

    existing = findCartItem(id);

    if (quantity + (existing ? existing->quantity : 0) > p->stock)
    {
        printf("Only %d units are available.\n", p->stock);
        return;
    }

    if (existing != NULL)
    {
        existing->quantity += quantity;
    }
    else
    {
        CartNode *newNode = (CartNode *)malloc(sizeof(CartNode));

        if (newNode == NULL)
        {
            printf("Memory allocation failed.\n");
            return;
        }

        newNode->productId = id;
        newNode->quantity = quantity;
        newNode->next = cartHead;
        cartHead = newNode;
    }

    pushRecent(&recentStack, id);

    printf("\n%d x %s added to cart.\n", quantity, p->name);
}

/* ===================== CART SUBTOTAL ===================== */

float calculateSubtotal(void)
{
    CartNode *current = cartHead;
    float total = 0.0f;

    while (current != NULL)
    {
        Product *p = getProductById(current->productId);

        if (p != NULL)
            total += p->price * current->quantity;

        current = current->next;
    }

    return total;
}

/* ===================== VIEW CART ===================== */

void viewCart(void)
{
    CartNode *current = cartHead;
    float subtotal = 0.0f;

    printf("\n================ YOUR BEAUTY CART ================\n");

    if (current == NULL)
    {
        printf("Your cart is empty.\n");
        return;
    }

    printf("%-5s %-30s %-8s %-12s %-12s\n",
           "ID", "PRODUCT", "QTY", "PRICE", "SUBTOTAL");

    line();

    while (current != NULL)
    {
        Product *p = getProductById(current->productId);

        if (p != NULL)
        {
            float itemTotal = p->price * current->quantity;

            printf("%-5d %-30s %-8d Rs. %-8.2f Rs. %-8.2f\n",
                   p->id, p->name, current->quantity,
                   p->price, itemTotal);

            subtotal += itemTotal;
        }

        current = current->next;
    }

    line();

    printf("Cart products : %d\n", cartItemCount());
    printf("Subtotal      : Rs. %.2f\n", subtotal);
}

/* ===================== UPDATE CART ===================== */

void updateCart(void)
{
    int id;
    int quantity;
    Product *p;
    CartNode *node;

    if (cartHead == NULL)
    {
        printf("Cart is empty.\n");
        return;
    }

    viewCart();

    id = readInt("\nEnter Product ID to update: ");
    node = findCartItem(id);
    p = getProductById(id);

    if (node == NULL || p == NULL)
    {
        printf("Product is not in the cart.\n");
        return;
    }

    quantity = readInt("Enter new quantity (0 to remove): ");

    if (quantity < 0)
    {
        printf("Invalid quantity.\n");
        return;
    }

    if (quantity > p->stock)
    {
        printf("Only %d units are available.\n", p->stock);
        return;
    }

    if (quantity == 0)
    {
        CartNode *current = cartHead;
        CartNode *previous = NULL;

        while (current != NULL && current->productId != id)
        {
            previous = current;
            current = current->next;
        }

        if (current != NULL)
        {
            if (previous == NULL)
                cartHead = current->next;
            else
                previous->next = current->next;

            free(current);
            printf("Item removed from cart.\n");
        }
    }
    else
    {
        node->quantity = quantity;
        printf("Cart quantity updated.\n");
    }
}

/* ===================== REMOVE FROM CART ===================== */

void removeFromCart(void)
{
    int id;
    CartNode *current;
    CartNode *previous;

    if (cartHead == NULL)
    {
        printf("Cart is empty.\n");
        return;
    }

    viewCart();

    id = readInt("\nEnter Product ID to remove: ");

    current = cartHead;
    previous = NULL;

    while (current != NULL && current->productId != id)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Product not found in cart.\n");
        return;
    }

    if (previous == NULL)
        cartHead = current->next;
    else
        previous->next = current->next;

    free(current);

    printf("Product removed from cart.\n");
}

/* ===================== CLEAR CART ===================== */

void clearCart(void)
{
    CartNode *current = cartHead;

    while (current != NULL)
    {
        CartNode *temp = current;
        current = current->next;
        free(temp);
    }

    cartHead = NULL;
}

/* ===================== COUPON SYSTEM ===================== */

float applyCoupon(float subtotal)
{
    char coupon[30];
    float discount = 0.0f;

    if (subtotal < 500)
    {
        printf("Coupons require a minimum purchase of Rs. 500.\n");
        return 0.0f;
    }

    printf("\n========== BEAUTY STORE COUPONS ==========\n");
    printf("GLOW10  -> 10%% off (minimum Rs. 1000)\n");
    printf("GLOW20  -> 20%% off (minimum Rs. 3000)\n");
    printf("BEAUTY500 -> Rs. 500 off (minimum Rs. 4000)\n");

    printf("\nEnter coupon or NONE: ");

    fgets(coupon, sizeof(coupon), stdin);
    coupon[strcspn(coupon, "\n")] = '\0';

    {
        int i;
        for (i = 0; coupon[i]; i++)
            coupon[i] = (char)toupper((unsigned char)coupon[i]);
    }

    if (strcmp(coupon, "GLOW10") == 0)
    {
        if (subtotal >= 1000)
            discount = subtotal * 0.10f;
        else
            printf("GLOW10 requires a minimum purchase of Rs. 1000.\n");
    }
    else if (strcmp(coupon, "GLOW20") == 0)
    {
        if (subtotal >= 3000)
            discount = subtotal * 0.20f;
        else
            printf("GLOW20 requires a minimum purchase of Rs. 3000.\n");
    }
    else if (strcmp(coupon, "BEAUTY500") == 0)
    {
        if (subtotal >= 4000)
            discount = 500.0f;
        else
            printf("BEAUTY500 requires a minimum purchase of Rs. 4000.\n");
    }
    else if (strcmp(coupon, "NONE") == 0)
    {
        discount = 0.0f;
    }
    else
    {
        printf("Invalid coupon.\n");
    }

    return discount;
}

/* ===================== FILE HANDLING ===================== */

void saveOrderToFile(Order *o, float subtotal,
                     float discount, float delivery)
{
    FILE *fp = fopen("beauty_orders.txt", "a");

    if (fp == NULL)
    {
        printf("Warning: Could not save order to file.\n");
        return;
    }

    fprintf(fp,
            "Order ID: %d | Subtotal: Rs. %.2f | "
            "Discount: Rs. %.2f | Delivery: Rs. %.2f | "
            "Total: Rs. %.2f | Status: %s\n",
            o->orderId, subtotal, discount,
            delivery, o->amount, o->status);

    fclose(fp);
}

void displaySavedOrders(void)
{
    FILE *fp;
    char lineText[400];

    fp = fopen("beauty_orders.txt", "r");

    if (fp == NULL)
    {
        printf("\nNo saved orders found.\n");
        return;
    }

    printf("\n================ SAVED BEAUTY ORDERS ================\n");

    while (fgets(lineText, sizeof(lineText), fp) != NULL)
        printf("%s", lineText);

    fclose(fp);
    line();
}

/* ===================== ORDER HISTORY ===================== */

void displayOrderHistory(void)
{
    int i;

    printf("\n================ ORDER HISTORY ================\n");

    if (orderCount == 0)
    {
        printf("No orders placed during this session.\n");
        printf("Previously saved orders can be viewed from file.\n");
        return;
    }

    printf("%-10s %-15s %-20s\n",
           "ORDER ID", "AMOUNT", "STATUS");

    line();

    for (i = 0; i < orderCount; i++)
    {
        printf("%-10d Rs. %-11.2f %-20s\n",
               orders[i].orderId,
               orders[i].amount,
               orders[i].status);
    }

    line();
}

/* ===================== CHECKOUT ===================== */

void checkout(void)
{
    float subtotal;
    float discount;
    float delivery;
    float total;

    int paymentChoice;
    int confirmChoice;

    char customerName[80];
    char address[200];

    if (cartHead == NULL)
    {
        printf("\nYour cart is empty.\n");
        return;
    }

    if (orderCount >= MAX_ORDERS)
    {
        printf("\nOrder storage is full.\n");
        return;
    }

    if (isQueueFull(&orderQueue))
    {
        printf("\nOrder processing queue is full.\n");
        printf("Please process existing orders first.\n");
        return;
    }

    viewCart();
    subtotal = calculateSubtotal();

    printf("\nProceeding to beauty checkout...\n");

    printf("\nEnter customer name: ");
    fgets(customerName, sizeof(customerName), stdin);
    customerName[strcspn(customerName, "\n")] = '\0';

    if (strlen(customerName) == 0)
    {
        printf("Customer name cannot be empty.\n");
        return;
    }

    printf("Enter delivery address: ");
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = '\0';

    if (strlen(address) == 0)
    {
        printf("Delivery address cannot be empty.\n");
        return;
    }

    discount = applyCoupon(subtotal);

    /* Free delivery above Rs. 999. */
    if (subtotal >= 999)
        delivery = 0.0f;
    else
        delivery = 99.0f;

    total = subtotal - discount + delivery;

    printf("\n================ BEAUTY ORDER BILL ================\n");
    printf("Customer     : %s\n", customerName);
    printf("Address      : %s\n", address);
    printf("Subtotal     : Rs. %.2f\n", subtotal);
    printf("Discount     : Rs. %.2f\n", discount);
    printf("Delivery     : Rs. %.2f\n", delivery);

    if (delivery == 0.0f)
        printf("Delivery     : FREE\n");

    printf("-----------------------------------------------\n");
    printf("TOTAL        : Rs. %.2f\n", total);
    printf("-----------------------------------------------\n");

    printf("\n========== PAYMENT METHOD ==========\n");
    printf("1. UPI\n");
    printf("2. Debit/Credit Card\n");
    printf("3. Cash on Delivery\n");

    paymentChoice = readInt("Choose payment method: ");

    if (paymentChoice < 1 || paymentChoice > 3)
    {
        printf("Invalid payment method.\n");
        return;
    }

    if (paymentChoice == 1)
        printf("UPI payment selected.\n");
    else if (paymentChoice == 2)
        printf("Card payment selected.\n");
    else
        printf("Cash on Delivery selected.\n");

    printf("\n1. Confirm Order\n");
    printf("2. Cancel\n");

    confirmChoice = readInt("Enter choice: ");

    if (confirmChoice != 1)
    {
        printf("Order cancelled.\n");
        return;
    }

    /*
       Final stock verification before payment/order placement.
    */
    {
        CartNode *current = cartHead;

        while (current != NULL)
        {
            Product *p = getProductById(current->productId);

            if (p == NULL)
            {
                printf("Invalid product found in cart.\n");
                return;
            }

            if (current->quantity > p->stock)
            {
                printf("\nInsufficient stock for: %s\n", p->name);
                printf("Available: %d\n", p->stock);
                return;
            }

            current = current->next;
        }
    }

    orders[orderCount].orderId = nextOrderId;
    orders[orderCount].amount = total;
    strcpy(orders[orderCount].status, "Processing");

    if (!enqueue(&orderQueue, orders[orderCount].orderId))
    {
        printf("Unable to add order to processing queue.\n");
        return;
    }

    /* Reduce stock after successful order confirmation. */
    {
        CartNode *current = cartHead;

        while (current != NULL)
        {
            Product *p = getProductById(current->productId);

            if (p != NULL)
                p->stock -= current->quantity;

            current = current->next;
        }
    }

    saveOrderToFile(&orders[orderCount],
                    subtotal, discount, delivery);

    printf("\n=================================================\n");
    printf("             ORDER PLACED SUCCESSFULLY           \n");
    printf("=================================================\n");
    printf("Order ID : %d\n", orders[orderCount].orderId);
    printf("Amount   : Rs. %.2f\n", orders[orderCount].amount);
    printf("Status   : %s\n", orders[orderCount].status);

    if (delivery == 0.0f)
        printf("Delivery : FREE\n");
    else
        printf("Delivery : Rs. %.2f\n", delivery);

    printf("\nThank you for shopping with GlowCart!\n");

    orderCount++;
    nextOrderId++;

    clearCart();
}

/* ===================== PROCESS NEXT ORDER ===================== */

void processNextOrder(void)
{
    int orderId;
    int i;

    orderId = dequeue(&orderQueue);

    if (orderId == -1)
    {
        printf("\nNo orders waiting for processing.\n");
        return;
    }

    for (i = 0; i < orderCount; i++)
    {
        if (orders[i].orderId == orderId)
        {
            strcpy(orders[i].status, "Shipped");

            printf("\n========== ORDER PROCESSED ==========\n");
            printf("Order ID : %d\n", orders[i].orderId);
            printf("Amount   : Rs. %.2f\n", orders[i].amount);
            printf("Status   : %s\n", orders[i].status);

            return;
        }
    }

    printf("Order could not be found.\n");
}

/* ===================== UPDATE ORDER STATUS ===================== */

void updateOrderStatus(void)
{
    int id;
    int choice;
    int i;
    int found = 0;

    if (orderCount == 0)
    {
        printf("\nNo orders available.\n");
        return;
    }

    id = readInt("Enter Order ID: ");

    for (i = 0; i < orderCount; i++)
    {
        if (orders[i].orderId == id)
        {
            found = 1;

            printf("\nCurrent Status: %s\n", orders[i].status);
            printf("\n1. Processing\n");
            printf("2. Shipped\n");
            printf("3. Delivered\n");
            printf("4. Cancelled\n");

            choice = readInt("Choose new status: ");

            switch (choice)
            {
                case 1: strcpy(orders[i].status, "Processing"); break;
                case 2: strcpy(orders[i].status, "Shipped"); break;
                case 3: strcpy(orders[i].status, "Delivered"); break;
                case 4: strcpy(orders[i].status, "Cancelled"); break;
                default:
                    printf("Invalid choice.\n");
                    return;
            }

            printf("Order status updated successfully.\n");
            break;
        }
    }

    if (!found)
        printf("Order ID not found.\n");
}

/* ===================== CART MENU ===================== */

void cartMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n========== BEAUTY SHOPPING CART ==========\n");
        printf("1. View Cart\n");
        printf("2. Add Product\n");
        printf("3. Update Quantity\n");
        printf("4. Remove Product\n");
        printf("5. Checkout\n");
        printf("6. Back\n");

        choice = readInt("Enter choice: ");

        switch (choice)
        {
            case 1:
                viewCart();
                pauseScreen();
                break;

            case 2:
                addToCart();
                pauseScreen();
                break;

            case 3:
                updateCart();
                pauseScreen();
                break;

            case 4:
                removeFromCart();
                pauseScreen();
                break;

            case 5:
                checkout();
                pauseScreen();
                break;

            case 6:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}

/* ===================== ORDER MENU ===================== */

void orderMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n========== BEAUTY ORDER MANAGEMENT ==========\n");
        printf("1. Order History\n");
        printf("2. Process Next Order\n");
        printf("3. View Processing Queue\n");
        printf("4. Update Order Status\n");
        printf("5. View Saved Orders From File\n");
        printf("6. Back\n");

        choice = readInt("Enter choice: ");

        switch (choice)
        {
            case 1:
                displayOrderHistory();
                pauseScreen();
                break;

            case 2:
                processNextOrder();
                pauseScreen();
                break;

            case 3:
                displayQueue(&orderQueue);
                pauseScreen();
                break;

            case 4:
                updateOrderStatus();
                pauseScreen();
                break;

            case 5:
                displaySavedOrders();
                pauseScreen();
                break;

            case 6:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}

/* ===================== ADMIN MENU ===================== */

void adminMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n========== BEAUTY STORE ADMIN TOOLS ==========\n");
        printf("1. View Product Stock\n");
        printf("2. Process Next Order\n");
        printf("3. Update Order Status\n");
        printf("4. View Order Queue\n");
        printf("5. View Order History\n");
        printf("6. Back\n");

        choice = readInt("Enter choice: ");

        switch (choice)
        {
            case 1:
                displayProducts(products, productCount);
                pauseScreen();
                break;

            case 2:
                processNextOrder();
                pauseScreen();
                break;

            case 3:
                updateOrderStatus();
                pauseScreen();
                break;

            case 4:
                displayQueue(&orderQueue);
                pauseScreen();
                break;

            case 5:
                displayOrderHistory();
                pauseScreen();
                break;

            case 6:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}

/* ===================== FREE MEMORY ===================== */

void freeMemory(void)
{
    CartNode *current = cartHead;

    while (current != NULL)
    {
        CartNode *temp = current;
        current = current->next;
        free(temp);
    }

    cartHead = NULL;
}

/* ===================== MAIN MENU ===================== */

void mainMenu(void)
{
    int choice;

    while (1)
    {
        printHeader();

        printf("1. Browse Beauty Products\n");
        printf("2. Search Products - Linear Search\n");
        printf("3. Sort Products - Bubble Sort\n");
        printf("4. Binary Search Product ID\n");
        printf("5. Beauty Shopping Cart\n");
        printf("6. Recently Viewed Products - Stack\n");
        printf("7. Orders - Queue & History\n");
        printf("8. Admin / Order Tools\n");
        printf("9. Exit\n");

        line();

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
            case 1:
                browseProducts();
                break;

            case 2:
                searchProducts();
                pauseScreen();
                break;

            case 3:
                sortProducts();
                pauseScreen();
                break;

            case 4:
                binarySearchById();
                pauseScreen();
                break;

            case 5:
                cartMenu();
                break;

            case 6:
                displayRecent(&recentStack);
                pauseScreen();
                break;

            case 7:
                orderMenu();
                break;

            case 8:
                adminMenu();
                break;

            case 9:
                printf("\n=================================================\n");
                printf("       THANK YOU FOR USING GLOWCART STORE        \n");
                printf("=================================================\n");
                return;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

/* ===================== MAIN ===================== */

int main(void)
{
    initStack(&recentStack);
    initQueue(&orderQueue);

    mainMenu();

    freeMemory();

    return 0;
}
