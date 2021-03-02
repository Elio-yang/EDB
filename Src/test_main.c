/*
 * @author Elio Yang
 * @email  jluelioyang2001@gamil.com
 * @date 2021/2/27
 */

#include <list.h>
#include <defs.h>

typedef struct stu{
        char name[20];
        int id;
        list_entry_t *link;
}stu_t;

typedef struct tea{
        char name[20];
        int id;
        double salary;
        list_entry_t *link;
}tea_t;


int main()
{
        stu_t stu1={
                .name="elio",
                .id=1,
                .link=nullptr
        };

        list_entry_t *listhead=stu1.link;
        list_init(listhead);
        stu_t stu2={
                .name="flio",
                .id=1,
                .link=nullptr
        };
        stu_t stu3={
                .name="glio",
                .id=1,
                .link=nullptr
        };
        stu_t stu4={
                .name="hlio",
                .id=1,
                .link=nullptr
        };
        tea_t tea1={
                .name="Elio",
                .id=1,
                .salary=100,
                .link=nullptr
        };
        tea_t tea2={
                .name="Flio",
                .id=2,
                .salary=100,
                .link=nullptr
        };
        tea_t tea3={
                .name="Glio",
                .id=3,
                .salary=100,
                .link=nullptr
        };
        list_add(listhead,stu2.link);
        list_add(listhead,stu3.link);
        list_add(listhead,stu4.link);
        list_add(listhead,tea1.link);
        list_add(listhead,tea2.link);
        list_add(listhead,tea3.link);

        list_entry_t *s=listhead;
        while (s->next!=listhead){
                __container_of__(s,stu_t,name);
        }




        return 0;
}