//reset is never called in this text... the conditional that chooses
//to reset must be somewhere else
void reset()
{
	::reset();
	herb_takers = ({});
	if(present("archer")) return;
	if(refs(get_object(LOTH_MON + "elf_archer")) > 10) return;
	if(random(4)) clone_object(LOTH_MON + "elf_archer")->move(this_object());
	else return;
	if(random(2)) clone_object(LOTH_MON + "elf_archer")->move(this_object());
	else return;
	if(!random(5)) clone_object(LOTH_MON + "elf_archer")->move(this_object());
}

int search_room(string str)
{
	//object that will become a flower
	object ob;
	//will become a players name
	string name;

	// if your players total skill of wilderness and lore > 50
	//    you have a random chance of getting this
	// if > 200 you get it 100% of the time
	if ( random((int)this_player()->query_skill("wilderness") +
	(int)this_player()->query_skill("nature's lore")) > 50+random(150)) {

		// set the players name
		name = (string)this_player()->query("name");

		// you can't take more than 1
		// if at least 6 people have taken herbs, you can't take any
		if (sizeof(herb_takers) > 5 ||
			member_array(name, herb_takers) != -1) {
			write("Alas, the flowers seem to have all been picked.\n");
			return 1;
		}

		// random chance to get one of these 2 flowers
		if (random(2) == 0)
			ob = clone_object("/obj/herbs/niphredil")
		else
			ob = clone_object("/obj/herbs/elanor");

		//you get the flower and skill
		ob->move(this_object());
		write("You walk up to a patch of sunlight, and spot "+ob->query("short")+"!\n"+
			"You quickly pick the delicate flower.\n");
		this_player()->improve_skill("nature's lore",2);

		//you get added to herb_takers
		herb_takers += ({ name });
		return 1;
	}
	//other stuff..?
	//the function doesn't seem to end
