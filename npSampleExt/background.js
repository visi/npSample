// sogouExplorer.bookmarks.onCreated.addListener(function(id, bm) {
// 	console.log("xx");
// 	console.log(id);
// 	console.log(bm);
// 	console.log("yy");
// });


// sogouExplorer.bookmarks.create({url: "http://123.com", title: "123"}, function(bm){});

// sogouExplorer.cookies.set({url: "http://www.sohu.com", name: "sohu"}, function(info) {
//     console.log(info)
// });




var bkgnd = chrome.extension.getBackgroundPage();
var sample = bkgnd.document.getElementById("Sample");

function getClickHandler(type) {
	return function(info, tab) {
		var url = info.pageUrl;
		var title = tab.title;

		if(type == "page") {
			sample.sayHello();
		}
	}
}

chrome.contextMenus.create({ "title" : "sayHello", "type" : "normal", "contexts" : [ "page" ], "onclick" : getClickHandler("page") });
