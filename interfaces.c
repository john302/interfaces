#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/sockios.h> // SIOCGIFHWADDR

#define MAX_INTERFACES 16

int main() {
    int sock;
    struct ifreq ifr[MAX_INTERFACES];
    struct ifconf ifc;

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    // Get the interface list
    ifc.ifc_len = sizeof(ifr);
    ifc.ifc_buf = (char *)ifr;
    if (ioctl(sock, SIOCGIFCONF, &ifc) < 0) {
        perror("ioctl (SIOCGIFCONF)");
        close(sock);
        return 1;
    }

    printf("+-----------------+----------------------+---------------------------+------------------+\n");
    printf("| Interface Name  | IP Address           | MAC Address               | Netmask          |\n");
    printf("+-----------------+----------------------+---------------------------+------------------+\n");

    int num_interfaces = ifc.ifc_len / sizeof(struct ifreq);
    for (int i = 0; i < num_interfaces; i++) {
        struct ifreq *item = &ifr[i];

        printf("| %-15s | ", item->ifr_name);

        // Get IP address
        if (ioctl(sock, SIOCGIFADDR, item) < 0) {
            printf("%-20s | ", "N/A"); // Handle case with no IP
        } else {
            struct sockaddr_in *ipaddr = (struct sockaddr_in *)&item->ifr_addr;
            printf("%-20s | ", inet_ntoa(ipaddr->sin_addr));
        }

        // Get MAC address
        if (ioctl(sock, SIOCGIFHWADDR, item) < 0) {
            printf("%-25s | ", "N/A");
        } else {
            unsigned char *mac = (unsigned char *)item->ifr_hwaddr.sa_data;
            printf("%02X:%02X:%02X:%02X:%02X:%02X        | ", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        }

        // Get Netmask
        if (ioctl(sock, SIOCGIFNETMASK, item) < 0) {
            printf("%-16s |\n", "N/A");
        } else {
            struct sockaddr_in *netmask = (struct sockaddr_in *)&item->ifr_netmask;
            printf("%-16s |\n", inet_ntoa(netmask->sin_addr));
        }
    }

    printf("+-----------------+----------------------+---------------------------+------------------+\n");

    close(sock);
    return 0;
}
