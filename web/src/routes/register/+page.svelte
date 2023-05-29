<style>
.container {
    display: flex;
    align-items: center;
    justify-content: center;
    position: relative;
    width: 100%;
    height: 90vh; 
    background: #0e0e10;
}

input {
    color-scheme: dark;
    appearance: none;
    background-clip: padding-box;
    line-height: 1.5;
    transition: border var(--timing-short) ease-in,background-color var(--timing-short) ease-in;
    border-style: solid;
    border-width: 2px;
    border-color: hsla(0,0%,100%,.16);
    background-color: hsla(0,0%,100%,.16);
    border-radius: 10px;
    margin: 5px;
}

.center {
    position: relative;
    background: #2e2e35;
    padding: 2rem;
    box-shadow: 0px 0px 30px gray;
    border-radius: 30px;
    color: white;
    text-align: center;
}

input:hover {
    outline: none;
    border-color: hsla(0,0%,100%,.28);
}

input:focus {
    border-color: #9146ff;
    outline: none;
    background-color: black;
}

</style>


<div class="container">
    <div class="center">
        <h2>Регистрация</h2>
        <form on:submit|preventDefault={onsubmit}>
            <input placeholder="Email" type="email" name="username"><br/>
            <input placeholder="Пароль" type="password" name="password"><br/>
            <input placeholder="Повтор пароля" type="password" name="password_rep"><br/>
            <input type="submit" value="Регистрация">
        </form>
    </div>
</div>
<p>{error}</p>

<script>
    import {call} from "../../rpc"
    import {goto} from "$app/navigation"

    let error = "";

    function onsubmit(event){
        const formData = new FormData(event.target);
		const data = {};
		for (let field of formData) {
			const [key, value] = field;
			data[key] = value;
		}
        call("/api/User/register",data,(data)=>{
            console.log(data)
            if(data){
                console.log(data["error"])
                error = data["error"];
            }else{
                // window.location.href = "/login"
                goto("/login");
            }
        },"POST")
    }
</script>