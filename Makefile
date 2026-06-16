NAME := ft_ping

CC := cc

SRC_DIR := src
INC_DIR := include
BUILD_DIR := .build

CFLAGS := -Wall -Wextra -Werror
CPPFLAGS := -I$(INC_DIR) -MMD -MP

LDFLAGS := 
LDLIBS := $(addprefix -l, m)

SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

DOCKER_IMAGE := $(NAME)
DOCKER_FLAGS := --cap-add=NET_RAW --rm -it -v $(shell pwd):/app

.PHONY: all clean fclean re docker-build-dev docker-dev docker-clean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

docker-build-dev:
	docker build --target dev -t $(DOCKER_IMAGE):dev .

docker-dev: docker-build-dev
	docker run $(DOCKER_FLAGS) $(DOCKER_IMAGE):dev

docker-clean:
	docker rmi -f $(DOCKER_IMAGE):dev
