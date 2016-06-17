void charToString(char*buffer,char c){
	buffer[0]=c;
	buffer[1]=0;
}

int intToString(char * buffer,int num){
	int i=0;
	int tmp=0;
	int k,q;
	if (num == 0) {
		buffer[0] = '0';
		return 1;
	}
	while(num){
		tmp = num/10;
		buffer[i]=(num - tmp*10)+'0';
		num=tmp;
		i++;
	}
	buffer[i]=0;
	for(k=0,q=i-1;k<q;k++,q--){
		tmp=buffer[k];
		buffer[k]=buffer[q];
		buffer[q]=tmp;
	}
	return i;
}

int strlen(const char * s){
	int i=0;
	while(*s){
		i++;
		s++;
	}
	return i;
}
