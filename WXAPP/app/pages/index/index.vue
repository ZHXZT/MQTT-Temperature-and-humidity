<template>
	<view class="content">
		<view class="">温度：{{temp}}</view>
		<view>湿度：{{humi}}</view>
		<switch :checked="led" @change=""></switch>
	</view>
</template>

<script>
import { get } from 'http'
	const{createCommonToken} = require('@/key.js')
	export default {
		data() {
			return {
				title: 'Hello',
				token:'',
				temp:'',
				humi:'',
				led:true
			}
		},
		
		
		onLoad() {
			const params = {
				author_key:'Tdrmjgudeg7ApsNQTY/NbUakU+WQN4OdxmBgd4micSOrwXa1aXWrICFpxOEkCPg0/pMFQf+AIE63+8Kn39XpUA==',
				user_id:'356541',
				version:'2022-05-01'
			}
			this.token = createCommonToken(params);
			
							this.getData()

		},
		
		
		
		methods: {
			getData(){
				uni.request({
					    url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', 
						methods:'GET',
					    data: {
					        product_id: 'AyKahgJ0Zx',
							device_name: 'a1'
					    },
					    header: {
					        'authorization': this.token
					    },
					    success: (res) => {
					        console.log(res.data);
							this.temp = res.data.data[2].value
							this.humi = res.data.data[0].value
							this.led = res.data.data[1].value == "true" ? true : false
					    }
					});
			},
			// setSwitch(){
			// 	uni.request({
			// 		    url: 'http://iot-api.heclouds.com/thingmodel/set-device-property', 
			// 			methods:'POST',
			// 		    data: {
			// 		        product_id: 'AyKahgJ0Zx',
			// 				device_name: 'a1',
			// 				params:{
			// 					"led":false
			// 				}
			// 		    },
			// 		    header: {
			// 		        'authorization': this.token
			// 		    },
			// 		    success: (res) => {
			// 		        console.log(res.data);
			// 		    }
			// 		});
			// }
		}
	}
</script>

<style>
	.content {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
	}

	.logo {
		height: 200rpx;
		width: 200rpx;
		margin-top: 200rpx;
		margin-left: auto;
		margin-right: auto;
		margin-bottom: 50rpx;
	}

	.text-area {
		display: flex;
		justify-content: center;
	}

	.title {
		font-size: 36rpx;
		color: #8f8f94;
	}
</style>
