#include<stdlib.h>
#include<string.h>
#include<stdio.h>
void main(){

char opcode[100],operand[100],address[100],label[100],opc[100],num[100],buffer1[100]="",buffer2[100]="",obj[100]="";
FILE *intfile,*optab,*symtab,*assembly,*objcode;
int stadd,count=0,temp,i,length;
intfile=fopen("intermediate.txt","r");
optab=fopen("optab.txt","r");
symtab=fopen("symtab.txt","r");
assembly=fopen("assembly.txt","w");
objcode=fopen("objectcode.txt","w");
for(fscanf(intfile,"%s\t%s\t%s\t%s",address,label,opcode,operand);;fscanf(intfile,"%s\t%s\t%s\t%s",address,label,opcode,operand))
{
printf("%s\t%s\t%s\t%s\n",address,label,opcode,operand);
if(strcmp(opcode,"START")==0)
    {   
        stadd=atoi(operand);
        fprintf(objcode,"h^%06s^%06s\n",opcode,operand);
        fprintf(assembly,"%s\t%s\t%s\t%s\n",address,label,opcode,operand);
    }
else if(strcmp(opcode,"END")==0){fprintf(assembly,"%s\t%s\t%s\t%s\n",address,label,opcode,operand);
                                    break;}
else if(strcmp(opcode,"RESW")==0 || strcmp(opcode,"RESB")==0){fprintf(assembly,"%s\t%s\t%s\t%s\n",address,label,opcode,operand);
                                                                continue;}
else
    {  
        if(count==0)
        {length=atoi(address);
            /*sprintf(buffer1,"t^%06s^00^",address);
        sprintf(buffer2,"^");*/}        
       for(fscanf(optab,"%s\t%02s",opc,num);;fscanf(optab,"%s\t%02s",opc,num)) 
            {
                if(strcmp(opc,opcode)==0){sprintf(buffer2+strlen(buffer2),"%02s",num);
                                          sprintf(obj+strlen(obj),"%02s",num);
                                          rewind(optab);
                                          break;}
                else if(strcmp(opcode,"WORD")==0){temp =1;
                                                break;}
                else if(strcmp(opcode,"BYTE")==0){temp=2;
                                                break;}
                if(feof(optab)){printf("error1");
                                break;         }                
            }
        for(fscanf(symtab,"%s\t%s",opc,num);;fscanf(symtab,"%s\t%s",opc,num))
            {   
                if(strcmp(opc,operand)==0){
                                            sprintf(buffer2+strlen(buffer2),"%04s^",num);
                                            sprintf(obj+strlen(obj),"%04s",num);
                                            count+=3;
                                            rewind(symtab);
                                            break;   }
                else if(temp==1){
                                sprintf(buffer2+strlen(buffer2),"%06s^",operand);
                                sprintf(obj+strlen(obj),"%06s",operand);
                                count+=3;
                                break;
                                }
                else if(temp==2){
                                for(i=2;i<strlen(operand)-1;i++)
                                    {
                                        sprintf(buffer2 + strlen(buffer2), "%02X", operand[i]);
                                        sprintf(obj+strlen(obj),"%02X",operand[i]);
                                        count++;                                      
                                    }
                                    sprintf(buffer2+strlen(buffer2),"^");
                                    break;
                                }
                if(feof(symtab)){printf("error2");
                                break;         }                
            }
       // sprintf(buffer1+9,"%02X",count);
        if(count>30){count=0;
                   
                    fprintf(objcode,"t^%06d^%02X^%s\b\n",length,count,buffer2);}

        fprintf(assembly,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,obj);
        sprintf(obj,"");
    }
}
fprintf(objcode,"t^%06d^%02X^%s\n",length,count,buffer2);
//fprintf(objcode,"%s\n",strcat(buffer1,buffer2));
fprintf(objcode,"e^%06d",stadd);

}