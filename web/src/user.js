import {call} from "../src/rpc"

export function get_userinfo(callback){

    call("/api/User/info",{token: localStorage.getItem("token")}, (data)=>{
        if(data["error"]){
            console.log(data["error"]);
        }
        callback(data);
    });

}

export function create_tmp_user(callback){
    call("/api/User/create_tmp",null,(data)=>{
        if(data["error"]){
            console.log(data["error"]);
            return;
        }
        localStorage.setItem("token", data["token"]);
        callback();
    })
}