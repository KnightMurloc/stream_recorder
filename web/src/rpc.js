
export function call(url, params,callback = null, method = "GET") {
    const request = new XMLHttpRequest();

    let data = null;

    if(method === "GET" && params){
        url += "?"
        for (let key in params) {
            url += key + "=" + params[key] + "&";
        }
    }else if(method === "POST"){
        if(params["token"]){
            url += "?token=" + params["token"];
            delete params["token"];
        }
        data = JSON.stringify(params);
    }

    if(callback != null) {
        request.onreadystatechange = () => {
            if(request.readyState === 4) {

                try {
                    const response = JSON.parse(request.responseText)
                    callback(response);
                }catch(e) {
                    callback();
                }
            }
        }
    }else{
        request.onreadystatechange = null;
    }
    request.open(method, url,true);
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.send(data);
}