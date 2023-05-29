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

.center {
    position: relative;
    background: #2e2e35;
    padding: 2rem;
    box-shadow: 0px 0px 30px gray;
    border-radius: 30px;
    color: white;
    text-align: center;
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
		<h2>Вход</h2>

		<form on:submit|preventDefault={onsubmit}>
			<input placeholder="Email" type="email" name="username"><br/>
			<input placeholder="Пароль" type="password" name="password"><br/>
			<input value="Вход" type="submit">
		</form>

		<p>{error}</p>
	</div>
</div>

<script>
	import { prevent_default } from "svelte/internal";
	import {call} from "../../rpc"
	import { onMount } from 'svelte';

	let ls = null;
	let error = "";

  function onsubmit(event){
		const formData = new FormData(event.target);
		const data = {};
		for (let field of formData) {
			const [key, value] = field;
			data[key] = value;
		}
		call("/api/User/login",data,(data)=>{
			console.log(data)
			if(data["error"]){
				console.log(data["error"])
				error = data["error"];
				return;
			}

			ls.setItem("token",data["token"]);
			window.location.href = "/";
		},"POST");
  }

	onMount(() => {
        typeof localStorage !== `undefined` && (ls = localStorage);
    });
</script>