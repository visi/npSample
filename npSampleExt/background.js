// 通过ID获取对象
var sample = document.querySelector("#sample");

function getClickHandler(type) {
    return function(info, tab) {
        var url = info.pageUrl;
        var title = tab.title;

        if(type == "page") {
            sample.sayHello();
        }
    }
}

chrome.contextMenus.create({"title" : "sayHello",
                            "type" : "normal",
                            "contexts" : ["page"],
                            "onclick" : getClickHandler("page")});
