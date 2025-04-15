#include <algorithm>
#include <iostream>

int main() {
    int n;
    std::cin >> n;
    int arr[100];
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::sort(arr, arr + n);
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}