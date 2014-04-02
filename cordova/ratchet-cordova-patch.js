if (xhr.readyState === 4) {
    if( xhr.status === 200 || (xhr.status == 0 && options.url.indexOf("file:///") != -1) ) {
        success(xhr, options);
    } else {
        failure(options.url);
    }
}
