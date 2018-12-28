#include "olx.h"

Olx::Olx(QObject *parent) : QObject(parent)
{
    request = new Requesting;
    write = new Write;
}

Olx::~Olx()
{
    delete request;
    delete write;
}

void Olx::Start()
{
    QUrl startPageAdsForSaleApartments("https://olx.uz/nedvizhimost/kvartiry/prodazha/tashkent/");
    ParseOlx(request->pageText(startPageAdsForSaleApartments));
}

void Olx::ParseOlx(QByteArray html)
{
    OlxListingsPage listingsPage(html);
    QList<QUrl> addresses = listingsPage.listingAddresses();
    QListIterator<QUrl> list(addresses);
    while(list.hasNext())
    {
        QUrl address = list.next();
        QByteArray adHTML = request->pageText(address);
        if(adHTML.isEmpty()) continue;
        OlxListingPage page(adHTML, address);
        QMap<int, QString> listData = page.parseListingData();
        write->writeToExcel(listData, row);
        row++;
    }
    QUrl nextPage = listingsPage.nextListingPageUrl();
    if(!nextPage.isEmpty())
        ParseOlx(request->pageText(nextPage));
}


